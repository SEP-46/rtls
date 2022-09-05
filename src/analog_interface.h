#pragma once

#include "communication_interface.h"
#include "vector.h"

class AnalogInterface : public CommunicationInterface
{
public:
	AnalogInterface();

	virtual int Read( Vec3* data);

	virtual int Write( const Vec3& data);
};
