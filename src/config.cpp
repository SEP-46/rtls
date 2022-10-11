#include "config.h"

#include <json.hpp>
#include <iostream>
#include <fstream>

using namespace nlohmann;

template<typename T>
struct EnumAndName
{
	T value;
	const char* name;
};

static EnumAndName<TrilaterationSolverType> gTrilaterationSolverTypeEnum[] = {
	{ TrilaterationSolverType::BUILTIN, "builtin" },
	{ TrilaterationSolverType::BASIC, "basic" },
	{ TrilaterationSolverType::EKF, "ekf" },
};

static EnumAndName<OutputType> gOutputTypeEnum[] = {
	{ OutputType::UART, "uart" },
	{ OutputType::ANALOG, "analog" },
	{ OutputType::WEBSOCKET, "websocket" },
};

template <typename T, size_t Size>
static T ParseEnum( const std::string& type, EnumAndName<T>( &map )[Size] )
{
	for ( const EnumAndName<T>& enumAndName : map )
	{
		if ( enumAndName.name == type )
			return enumAndName.value;
	}

	std::cerr << "Unknown type '" << type << "'\n";
	return map[0].value;
}

template <typename T, size_t Size>
static std::string SaveEnum( T type, EnumAndName<T>( &map )[Size] )
{
	for ( const EnumAndName<T>& enumAndName : map )
	{
		if ( enumAndName.value == type )
			return enumAndName.name;
	}

	std::cerr << "Unknown type '" << (int)type << "'\n";
	return map[0].name;
}

static UartOutputConfig ParseUartOutputConfig( const json& data )
{
	UartOutputConfig uart;
	if ( data.find("portIndex") != data.end() )
		uart.portIndex = data["portIndex"];
	if ( data.find( "portName" ) != data.end() )
		uart.portName = data["portName"];
	return uart;
}

static json SaveUartOutputConfig( const UartOutputConfig& data )
{
	json uart;
	if ( data.portIndex != -1 )
		uart["portIndex"] = data.portIndex;
	if ( !data.portName.empty() )
		uart["portName"] = data.portName;
	return uart;
}

static WebSocketOutputConfig ParseWebSocketOutputConfig( const json& data )
{
	WebSocketOutputConfig ws;
	if ( data.find( "port" ) != data.end() )
		ws.port = data["port"];
	return ws;
}

static json SaveWebSocketOutputConfig( const WebSocketOutputConfig& data )
{
	json ws;
	if ( data.port != -1 )
		ws["port"] = data.port;
	return ws;
}

static OutputConfig ParseOutputConfig( const json& data )
{
	OutputConfig output;
	output.type = ParseEnum( data["type"], gOutputTypeEnum );
	if ( output.type == OutputType::UART )
		output.uart = ParseUartOutputConfig( data );
	else if ( output.type == OutputType::WEBSOCKET )
		output.websocket = ParseWebSocketOutputConfig( data );
	return output;
}

static json SaveOutputConfig( const OutputConfig& data )
{
	json output;
	if ( data.type == OutputType::UART )
		output = SaveUartOutputConfig( data.uart );
	if ( data.type == OutputType::WEBSOCKET )
		output = SaveWebSocketOutputConfig( data.websocket );
	output["type"] = SaveEnum( data.type, gOutputTypeEnum );
	return output;
}

static Vec3 ParseVector( const json& data )
{
	Vec3 vec;
	vec.x = data[0];
	vec.y = data[1];
	vec.z = data[2];
	return vec;
}

static json SaveVector( const Vec3& data )
{
	json vec = json::array();
	vec.push_back( data.x );
	vec.push_back( data.y );
	vec.push_back( data.z );
	return vec;
}

static AnchorConfig ParseAnchorConfig( const json& data )
{
	AnchorConfig anchor;
	anchor.id = data["id"];
	anchor.pos = ParseVector( data["pos"] );
	anchor.name = data["name"];
	return anchor;
}

static json SaveAnchorConfig( const AnchorConfig& data )
{
	json anchor;
	anchor["id"] = data.id;
	anchor["pos"] = SaveVector( data.pos );
	anchor["name"] = data.name;
	return anchor;
}

static TagConfig ParseTagConfig( const json& data )
{
	TagConfig tag;
	tag.name = data["name"];
	return tag;
}

static json SaveTagConfig( const TagConfig& data )
{
	json tag;
	tag["name"] = data.name;
	return tag;
}

static AABB ParseBounds( const json& data )
{
	AABB bounds;
	bounds.mins = ParseVector( data["mins"] );
	bounds.maxs = ParseVector( data["maxs"] );
	return bounds;
}

static json SaveBounds( const AABB& data )
{
	json bounds;
	bounds["mins"] = SaveVector( data.mins );
	bounds["maxs"] = SaveVector( data.maxs );
	return bounds;
}

Config LoadConfig( const char* filename )
{
	std::ifstream file( filename );
	json data = json::parse( file );

	Config cfg;
	
	cfg.trilaterationType = ParseEnum( data["trilaterationType"], gTrilaterationSolverTypeEnum );

	for ( const json& output : data["outputs"] )
	{
		cfg.outputs.push_back( ParseOutputConfig( output ) );
	}

	for ( const json& anchor : data["anchors"] )
	{
		cfg.anchors.push_back( ParseAnchorConfig( anchor ) );
	}

	cfg.tag = ParseTagConfig( data["tag"] );

	cfg.bounds = ParseBounds( data["bounds"] );

	return cfg;
}

void SaveConfig( const Config& cfg, const char* filename )
{
	json data;

	data["trilaterationType"] = SaveEnum( cfg.trilaterationType, gTrilaterationSolverTypeEnum );

	json outputs = json::array();
	for ( const OutputConfig& output : cfg.outputs )
	{
		outputs.push_back( SaveOutputConfig( output ) );
	}
	data["outputs"] = outputs;

	json anchors = json::array();
	for ( const AnchorConfig& anchor : cfg.anchors )
	{
		anchors.push_back( SaveAnchorConfig( anchor ) );
	}
	data["anchors"] = anchors;

	data["tag"] = SaveTagConfig( cfg.tag );

	data["bounds"] = SaveBounds( cfg.bounds );

	std::string str = data.dump();
	std::ofstream file( filename );
	file.write( str.c_str(), str.size() );
}