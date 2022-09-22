#include "restapi.h"
#include "rtls.h"
#include "settings.h"

#include <thread>

int main( int argc, const char* argv[] )
{
	InitSettings( argc, argv );

	RTLS rtls;
	std::thread webServer( StartWebServer, std::ref( rtls ) );
	while ( rtls.Run() )
		;
}
