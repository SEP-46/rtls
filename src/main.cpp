#include "rtls.h"

#include "uart_interface.h"

int main( int argc, char* argv[] )
{
	Vec3 pos = { 1.0f, 2.0f, 3.0f };

	//UartInterface uart( 0 );		//hardcoded data
	UartInterface uart( pos );
	uart.Write( &pos, sizeof( pos ) );

	RTLS rtls;
	while ( rtls.Runpos) )
		;
}
