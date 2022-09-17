#pragma once

#include "communication_interface.h"
#include "vector.h"
#include <cstdint>

struct Resolution
{
	float x;
	float y;
	float z;
};

class AnalogInterface : public CommunicationInterface
{
public:
	AnalogInterface();

	virtual int Read( Vec3* data);
	virtual int Write( const Vec3& data);

	Vec3 GetMinRange();
	Vec3 GetMaxRange();
	void SetRange(Vec3 aMin, Vec3 aMax);
	void SetResolution();

	uint16_t CastAsInt(float aResAxis, float aTagCoord);

private:
	Vec3 mMinRange = { -2.048, -2.048, -2.048 };
	Vec3 mMaxRange = { 2.048, 2.048, 2.048 };
	Resolution mResolution = { 1000.0, 1000.0, 1000.0 };
};
