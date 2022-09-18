#pragma once

struct Settings
{
	bool mock_tag = false;
};

extern void InitSettings( int argc, const char* argv[] );
extern const Settings& GetSettings();