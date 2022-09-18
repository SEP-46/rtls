#include "settings.h"

#include "optparse.h"

static Settings gSettings;

void InitSettings( int argc, const char* argv[] )
{
	OptParse opts;
	opts.AddFlagOption( "mock-tag" );

	opts.Process( argc, argv );

	gSettings.mock_tag = opts["mock-tag"];
}

const Settings& GetSettings()
{
	return gSettings;
}