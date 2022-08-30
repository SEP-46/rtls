#pragma once

#include "communication_interface.h"

class UartInterface : public CommunicationInterface
{
public:
	UartInterface( int port );

	virtual int Read( void* data, int length ) override;

	virtual int Write( const void* data, int length = 0 ) override;
};
