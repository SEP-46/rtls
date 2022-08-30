#include "uart_interface.h"
#include <serial/serial.h>

static serial::Serial gSerial;

UartInterface::UartInterface( int port )
{
	gSerial.setBaudrate( 115200 );
	gSerial.setBytesize( serial::eightbits );
	gSerial.setParity( serial::parity_none );
	gSerial.setStopbits( serial::stopbits_one );
	gSerial.setFlowcontrol( serial::flowcontrol_none );

	auto timeout = serial::Timeout::simpleTimeout( 100 );
	gSerial.setTimeout( timeout );

	std::vector<serial::PortInfo> ports = serial::list_ports();
	gSerial.setPort( ports[port].port );
}

int UartInterface::Read( void* data, int length )
{
	return (int)gSerial.read( (uint8_t*)data, length );
}

int UartInterface::Write( const void* data, int length )
{
	return (int)gSerial.write( (const uint8_t*)data, length );
}
