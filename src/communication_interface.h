#pragma once

#include "vector.h"

class CommunicationInterface
{
public:
	virtual bool Write( const Vec3& pos, const Vec3& vel ) = 0;
};