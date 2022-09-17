#pragma once

#include "vector.h"

class CommunicationInterface
{
public:
	virtual bool Read( Vec3* data ) = 0;
	virtual bool Write( const Vec3& data ) = 0;
};