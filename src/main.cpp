#include "rtls.h"

#include "uart_interface.h"

int main( int argc, char* argv[] )
{
	RTLS rtls;
	while ( rtls.Run() )
		;
}
