#include "restapi.h"
#include "rtls.h"
#include "settings.h"

#include <thread>

static RTLS rtls;

#ifdef WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

BOOL WINAPI CtrlHandler( DWORD fdwCtrlType )
{
	if ( fdwCtrlType == CTRL_C_EVENT || fdwCtrlType == CTRL_CLOSE_EVENT )
	{
		rtls.Stop();
		return TRUE;
	}

	return FALSE;
}
#else
#include <signal.h>

void CtrlHandler( int signum )
{
	rtls.Stop();
}
#endif

int main( int argc, const char* argv[] )
{
#ifdef WIN32
	SetConsoleCtrlHandler( CtrlHandler, TRUE );
#else
	signal( SIGINT, CtrlHandler );
#endif

	InitSettings( argc, argv );

	rtls.Init( GetSettings() );

	std::thread webServer( StartWebServer, std::ref( rtls ) );
	while ( rtls.Run() )
		;
}
