#include "rtls.h"
#include "settings.h"

int main( int argc, const char* argv[] )
{
	InitSettings( argc, argv );

	RTLS rtls;
	while ( rtls.Run() )
		;
}
