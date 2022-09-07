#include "uart_interface.h"
#include <serial/serial.h>

static serial::Serial gSerial;

UartInterface::UartInterface( int portIndex )
{
	std::vector<serial::PortInfo> ports = serial::list_ports();
	if ( portIndex < ports.size() )
		ConnectToPort( ports[portIndex].port.c_str() );
}

UartInterface::UartInterface( const char* portName )
{
	ConnectToPort( portName );
}

int UartInterface::Read( void* data, int length )
{
	if ( !gSerial.isOpen() )
		return 0;

	return (int)gSerial.read( (uint8_t*)data, length );
}

int UartInterface::Write( const void* data, int length )
{
	if ( !gSerial.isOpen() )
		return 0;

	return (int)gSerial.write( (const uint8_t*)data, length );
}

void UartInterface::ConnectToPort( const char* portName )
{
	gSerial.setBaudrate( 115200 );
	gSerial.setBytesize( serial::eightbits );
	gSerial.setParity( serial::parity_none );
	gSerial.setStopbits( serial::stopbits_one );
	gSerial.setFlowcontrol( serial::flowcontrol_none );

	auto timeout = serial::Timeout::simpleTimeout( 100 );
	gSerial.setTimeout( timeout );

	gSerial.setPort( portName );
	gSerial.open();
}
