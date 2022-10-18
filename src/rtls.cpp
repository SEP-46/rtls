#include "rtls.h"

#include "analog_interface.h"
#include "uart_interface.h"
#include "websocket_interface.h"
#include "settings.h"
#include "uwb_tag.h"
#include "mock_tag.h"
#include "trilaterationsolver_basic.h"
#include "trilaterationsolver_builtin.h"
#include "trilaterationsolver_ekf.h"
#include <iostream>
#include <filesystem>
#include <fstream>

using namespace std::string_literals;

RTLS::RTLS()
{
}

RTLS::~RTLS()
{
}

void RTLS::Init( const Settings& settings )
{
	if ( settings.mock_tag )
	{
		mTag = std::make_unique<MockTag>();
	}
	else
	{
		mTag = std::make_unique<UWBTag>();
	}

	mShouldLog = settings.log;

	Config cfg;
	if ( !settings.config_file.empty() )
	{
		cfg = LoadConfig( settings.config_file.c_str() );
	}
	else if ( std::filesystem::exists( "config.json" ) )
	{
		cfg = LoadConfig( "config.json" );
	}
	else
	{
		// Add all algorithms, but only output on basic algorithm
		{
			AlgorithmConfig alg;
			alg.trilaterationType = TrilaterationSolverType::BASIC;

			OutputConfig output;
			output.type = OutputType::WEBSOCKET;
			output.websocket.port = 9002;
			alg.outputs.push_back( output );

			cfg.algorithms.push_back( std::move( alg ) );
		}
		{
			AlgorithmConfig alg;
			alg.trilaterationType = TrilaterationSolverType::EKF;

			cfg.algorithms.push_back( std::move( alg ) );
		}
		{
			AlgorithmConfig alg;
			alg.trilaterationType = TrilaterationSolverType::BUILTIN;

			cfg.algorithms.push_back( std::move( alg ) );
		}

		cfg.bounds.mins = { 0.0f, 0.0f, 0.0f };
		cfg.bounds.maxs = { 10.0f, 10.0f, 10.0f };

		const UWBAnchorList& anchors = mTag->GetAnchorList();
		for ( size_t i = 0; i < anchors.NumAnchors(); i++ )
		{
			auto& anchor = anchors.GetAnchorByIndex( i );
			AnchorConfig anchorConfig;
			anchorConfig.id = anchor.GetId();
			anchorConfig.name = "A"s + std::to_string( anchor.GetId() );
			anchorConfig.pos = anchor.GetPosition();
			cfg.anchors.push_back( anchorConfig );
		}
	}

	ApplyConfig( cfg );
}

bool RTLS::Run()
{
	if ( mStopped )
		return false;

	if ( !mTag->ReadDistanceData() )
		return true;

	Vec3 anchorPositions[MAX_ANCHORS];
	float anchorDistances[MAX_ANCHORS];
	std::vector<AnchorDistanceMeasurement> measurements = mTag->CollectAnchorDistances();
	size_t numAnchors = measurements.size();
	for ( size_t i = 0; i < measurements.size(); i++ )
	{
		auto anchors = mTag->GetAnchorList();
		const AnchorDistanceMeasurement& measurement = measurements[i];
		AnchorConfig* anchorConfig = FindAnchorConfigById( measurement.id );
		AnchorConfig tagAnchorConfig;
		if ( !anchorConfig )
			anchorConfig = AddAnchorToConfig( measurement.id, "A" + std::to_string( measurement.id ), anchors.FindAnchorById( measurement.id )->GetPosition() );

		anchorPositions[i] = anchorConfig->pos;
		anchorDistances[i] = measurement.distance;
	}
	Timestamp_t timestamp = mTag->GetLastUpdatedTimestamp();

	if ( mShouldLog )
	{
		InputLogEntry entry;
		entry.timestamp = timestamp;
		entry.num_measurements = measurements.size();
		for ( size_t i = 0; i < entry.num_measurements; i++ )
		{
			entry.measurements[i] = measurements[i];
		}
		mInputLog.push_back( entry );
	}

	for ( Algorithm& algorithm : mAlgorithms )
	{
		TrilaterationResult result = algorithm.solver->FindTagPosition( timestamp, anchorPositions, anchorDistances, numAnchors );

		std::cout << "\nAlgorithm: " << algorithm.solver->GetName() << '\n';
		std::cout << "Tag position: " << result.position.x << ", " << result.position.y << ", " << result.position.z << '\n';
		std::cout << "Tag velocity: " << result.velocity.x << ", " << result.velocity.y << ", " << result.velocity.z << '\n';

		const Vec3& pos = result.position;
		const Vec3& vel = result.velocity;

		mVelocityOutputData.CalcVelocity( pos, timestamp );
		//mVelocityOutputData.TestPrintVelocity();

		for ( const auto& output : algorithm.outputs )
		{
			output->Write( pos, vel );
		}

		if ( mShouldLog )
		{
			OutputLogEntry entry;
			entry.timestamp = timestamp;
			entry.pos = pos;
			algorithm.log.push_back( entry );
		}
	}

	return true;
}

void RTLS::Stop()
{
	mStopped = true;

	SaveConfig();
	SaveLog();
}

std::string RTLS::GetTagName() const
{
	return mConfig.tag.name;
}

void RTLS::SetTagName( const std::string& name )
{
	mConfig.tag.name = name;
	SaveConfig();
}

std::vector<AnchorConfig> RTLS::GetAnchors() const
{
	return mConfig.anchors;
}

bool RTLS::SetAnchorName( NodeId_t id, const std::string& name )
{
	AnchorConfig* anchorConfig = FindAnchorConfigById( id );
	if ( !anchorConfig )
		return false;
	anchorConfig->name = name;

	SaveConfig();
	return true;
}

bool RTLS::SetAnchorPos( NodeId_t id, const Vec3& pos )
{
	AnchorConfig* anchorConfig = FindAnchorConfigById( id );
	if ( !anchorConfig )
		return false;
	anchorConfig->pos = pos;

	SaveConfig();
	return true;
}

void RTLS::ApplyConfig( const Config& cfg )
{
	mConfig = cfg;

	for ( const AlgorithmConfig& algorithmCfg : cfg.algorithms )
	{
		Algorithm algorithm;

		if ( algorithmCfg.trilaterationType == TrilaterationSolverType::BASIC )
			algorithm.solver = std::make_unique<TrilaterationSolver_Basic>();
		else if ( algorithmCfg.trilaterationType == TrilaterationSolverType::EKF )
			algorithm.solver = std::make_unique<TrilaterationSolver_EKF>();
		else if ( algorithmCfg.trilaterationType == TrilaterationSolverType::BUILTIN )
			algorithm.solver = std::make_unique<TrilaterationSolver_Builtin>( mTag.get() );

		for ( const OutputConfig& output : algorithmCfg.outputs )
		{
			std::unique_ptr<CommunicationInterface> interface;
			if ( output.type == OutputType::UART )
			{
				if ( !output.uart.portName.empty() )
					interface = std::make_unique<UartInterface>( output.uart.portName.c_str() );
				else
					interface = std::make_unique<UartInterface>( output.uart.portIndex );
			}
			else if ( output.type == OutputType::ANALOG )
			{
				auto analog = std::make_unique<AnalogInterface>();
				analog->SetBounds( mConfig.bounds );
				interface = std::move( analog );
			}
			else if ( output.type == OutputType::WEBSOCKET )
			{
				interface = std::make_unique<WebSocketInterface>( output.websocket.port );
			}

			algorithm.outputs.push_back( std::move( interface ) );
		}

		algorithm.solver->SetBounds( mConfig.bounds );

		mAlgorithms.push_back( std::move( algorithm ) );
	}
}

void RTLS::SaveConfig()
{
	::SaveConfig( mConfig, "config.json" );
}

AnchorConfig* RTLS::FindAnchorConfigById( NodeId_t id )
{
	for ( AnchorConfig& anchor : mConfig.anchors )
	{
		if ( anchor.id == id )
			return &anchor;
	}
	return nullptr;
}

AnchorConfig* RTLS::AddAnchorToConfig( NodeId_t id, const std::string& name, const Vec3& pos )
{
	AnchorConfig anchor;
	anchor.id = id;
	anchor.name = name;
	anchor.pos = pos;
	mConfig.anchors.push_back( anchor );
	return &mConfig.anchors.back();
}

void RTLS::SaveLog()
{
	time_t t = time( 0 );
	struct tm* now = localtime( &t );

	char datetime[80];
	strftime( datetime, sizeof( datetime ), "%F_%H%M%S", now);

	{
		std::string filename = "input_log_"s + datetime + ".csv"s;
		std::ofstream file( filename );
		for ( const InputLogEntry& entry : mInputLog )
		{
			file << entry.timestamp;
			for ( size_t i = 0; i < entry.num_measurements; i++ )
			{
				file << ',' << entry.measurements[i].id << ',' << entry.measurements[i].distance;
			}
			file << '\n';
		}
	}

	for ( const Algorithm& algorithm : mAlgorithms )
	{
		std::string filename = "output_log_"s + algorithm.solver->GetName() + '_' + datetime + ".csv"s;
		std::ofstream file( filename );
		for ( const OutputLogEntry& entry : algorithm.log )
		{
			file << entry.timestamp << ',' << entry.pos.x << ',' << entry.pos.y << ',' << entry.pos.z << '\n';
		}
	}
}

void RTLS::SetBounds( const AABB& bounds )
{
	for ( Algorithm& algorithm : mAlgorithms )
	{
		algorithm.solver->SetBounds( bounds );
		for ( std::unique_ptr<CommunicationInterface>& output : algorithm.outputs )
		{
			if ( auto* analog = dynamic_cast<AnalogInterface*>( output.get() ) )
			{
				analog->SetBounds( bounds );
			}
		}
	}

	mConfig.bounds = bounds;
	SaveConfig();
}
