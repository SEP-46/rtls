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
		cfg.trilaterationType = TrilaterationSolverType::BASIC;

		OutputConfig output;
		output.type = OutputType::WEBSOCKET;
		output.websocket.port = 9002;
		cfg.outputs.push_back( output );

		cfg.bounds.mins = { 0.0f, 0.0f, 0.0f };
		cfg.bounds.maxs = { 10.0f, 10.0f, 10.0f };
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
		const AnchorDistanceMeasurement& measurement = measurements[i];
		AnchorConfig* anchorConfig = FindAnchorConfigById( measurement.id );
		AnchorConfig tagAnchorConfig;
		if ( !anchorConfig )
		{
			const UWBAnchorList& anchors = mTag->GetAnchorList();
			const UWBAnchor* anchor = anchors.FindAnchorById( measurement.id );
			if ( !anchor )
				return false;
			tagAnchorConfig.id = anchor->GetId();
			tagAnchorConfig.pos = anchor->GetPosition();
			tagAnchorConfig.name = "A" + std::to_string( tagAnchorConfig.id );
			anchorConfig = &tagAnchorConfig;
		}

		anchorPositions[i] = anchorConfig->pos;
		anchorDistances[i] = measurement.distance;
	}
	Timestamp_t timestamp = mTag->GetLastUpdatedTimestamp();

	TrilaterationResult result = mTrilaterationSolver->FindTagPosition( timestamp, anchorPositions, anchorDistances, numAnchors );
	
	std::cout << "Tag position: " << result.position.x << ", " << result.position.y << ", " << result.position.z << '\n';
	std::cout << "Tag velocity: " << result.velocity.x << ", " << result.velocity.y << ", " << result.velocity.z << '\n';

	const Vec3& pos = result.position;
	const Vec3& vel = result.velocity;

	mVelocityOutputData.CalcVelocity( pos, timestamp );
	mVelocityOutputData.TestPrintVelocity();

	for ( const auto& output : mOutputInterfaces )
	{
		output->Write( pos, vel );
	}

	if ( mShouldLog )
	{
		LogEntry entry;
		entry.timestamp = timestamp;
		entry.pos = pos;
		entry.num_measurements = measurements.size();
		for ( size_t i = 0; i < entry.num_measurements; i++ )
		{
			entry.measurements[i] = measurements[i];
		}
		mLog.push_back( entry );
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

	if ( cfg.trilaterationType == TrilaterationSolverType::BASIC )
		mTrilaterationSolver = std::make_unique<TrilaterationSolver_Basic>();
	else if ( cfg.trilaterationType == TrilaterationSolverType::EKF )
		mTrilaterationSolver = std::make_unique<TrilaterationSolver_EKF>();
	else if ( cfg.trilaterationType == TrilaterationSolverType::BUILTIN )
		mTrilaterationSolver = std::make_unique<TrilaterationSolver_Builtin>( mTag.get() );

	for ( const OutputConfig& output : cfg.outputs )
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

		mOutputInterfaces.push_back( std::move( interface ) );
	}

	mTrilaterationSolver->SetBounds( mConfig.bounds );
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
		std::string filename = "output_log_"s + datetime + ".csv"s;
		std::ofstream file( filename );
		for ( const LogEntry& entry : mLog )
		{
			file << entry.timestamp << ',' << entry.pos.x << ',' << entry.pos.y << ',' << entry.pos.z << '\n';
		}
	}

	{
		std::string filename = "input_log_"s + datetime + ".csv"s;
		std::ofstream file( filename );
		for ( const LogEntry& entry : mLog )
		{
			file << entry.timestamp;
			for ( size_t i = 0; i < entry.num_measurements; i++ )
			{
				file << ',' << entry.measurements[i].id << ',' << entry.measurements[i].distance;
			}
			file << '\n';
		}
	}
}

void RTLS::SetBounds( const AABB& bounds )
{
	mTrilaterationSolver->SetBounds( bounds );
	for ( std::unique_ptr<CommunicationInterface>& output : mOutputInterfaces )
	{
		if ( auto* analog = dynamic_cast<AnalogInterface*>(output.get()) )
		{
			analog->SetBounds( bounds );
		}
	}

	mConfig.bounds = bounds;
	SaveConfig();
}
