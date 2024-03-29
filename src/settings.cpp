#include "settings.h"

#include "optparse.h"

static Settings gSettings;

void InitSettings( int argc, const char* argv[] )
{
	OptParse opts;
	opts.AddFlagOption( "mock-tag" )
		.AddFlagOption( "log" );
	opts.AddArgOption( "config", 'c' );

	opts.Process( argc, argv );

	gSettings.mock_tag = opts["mock-tag"];
	gSettings.config_file = opts["config"] ? opts["config"] : "";
	gSettings.log = opts["log"];
}

const Settings& GetSettings()
{
	return gSettings;
}