#pragma once

#include "aabb.h"
#include "uwb_anchor.h"
#include <string>
#include <vector>

enum class TrilaterationSolverType
{
	BUILTIN,
	BASIC,
	EKF
};

enum class OutputType
{
	UART,
	ANALOG,
	WEBSOCKET
};

struct UartOutputConfig
{
	int portIndex = -1;
	std::string portName;
};

struct WebSocketOutputConfig
{
	int port = -1;
};

struct OutputConfig
{
	OutputType type;

	UartOutputConfig uart;
	WebSocketOutputConfig websocket;
};

struct AnchorConfig
{
	NodeId_t id;
	std::string name;
	Vec3 pos;
};

struct TagConfig
{
	std::string name;
};

struct AlgorithmConfig
{
	TrilaterationSolverType trilaterationType;
	std::vector<OutputConfig> outputs;
};

struct Config
{
	std::vector<AlgorithmConfig> algorithms;
	std::vector<AnchorConfig> anchors;
	TagConfig tag;
	AABB bounds;
};

Config LoadConfig( const char* filename );
void SaveConfig( const Config& cfg, const char* filename );