#pragma once

#include <string>

struct Settings
{
	bool mock_tag = false;
	std::string config_file;
	bool log;
};

extern void InitSettings( int argc, const char* argv[] );
extern const Settings& GetSettings();