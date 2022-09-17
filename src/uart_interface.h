#pragma once

#include "communication_interface.h"

class UartInterface : public CommunicationInterface
{
public:
	UartInterface( int portIndex );
	UartInterface( const char* portName );

	virtual bool Read( Vec3* data ) override;
	virtual bool Write( const Vec3& data ) override;

private:
	void ConnectToPort( const char* portName );
};
