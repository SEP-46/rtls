#pragma once

#include "communication_interface.h"

class UartInterface : public CommunicationInterface
{
public:
	UartInterface( int portIndex );
	UartInterface( const char* portName );

	bool Write( const Vec3& pos, const Vec3& vel ) override;

private:
	void ConnectToPort( const char* portName );
};
