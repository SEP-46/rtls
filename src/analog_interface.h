#pragma once

#include "communication_interface.h"

class AnalogInterface : public CommunicationInterface
{
public:
	AnalogInterface();

	virtual int Read( void* data, int length ) override;

	virtual int Write( const void* data, int length ) override;
};
