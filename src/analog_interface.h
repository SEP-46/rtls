#pragma once

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

	Vec3 GetMinRange();
	Vec3 GetMaxRange();
	void SetRange(const Vec3& aMin, const Vec3& aMax);
	uint16_t Translate(float aValue, float aMinRange, float aMaxRange);

private:
	Vec3 mMinRange = { -2.048, -2.048, -2.048 };
	Vec3 mMaxRange = { 2.048, 2.048, 2.048 };
	Vec3 mSpanRange = { 4.095, 4.095, 4.095 };
};
