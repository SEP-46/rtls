#pragma once

#include "aabb.h"
#include "communication_interface.h"
#include "vector.h"
#include <cstdint>

class AnalogInterface : public CommunicationInterface
{
public:
	AnalogInterface();
	~AnalogInterface();

	virtual bool Read( Vec3* data);
	virtual bool Write( const Vec3& data);

	void SetBounds( const AABB& bounds ) { mBounds = bounds; }
	float Translate(float aValue, float aMinRange, float aMaxRange);

private:
	AABB mBounds;
};
