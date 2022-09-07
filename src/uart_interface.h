#pragma once

#include "communication_interface.h"

class UartInterface : public CommunicationInterface
{
public:
	UartInterface( int portIndex );
	UartInterface( const char* portName );

	virtual int Read( void* data, int length ) override;
	virtual int Write( const void* data, int length ) override;

private:
	void ConnectToPort( const char* portName );
};
