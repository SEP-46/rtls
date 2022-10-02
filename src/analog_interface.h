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
	float Translate(float aValue, float aMinRange, float aMaxRange);

private:
	Vec3 mMinRange = { 0.0f, 0.0f, 0.0f };
	Vec3 mMaxRange = { 1.0f, 1.0f, 1.0f };
	Vec3 mSpanRange = { 1.0f, 1.0f, 1.0f };
};
