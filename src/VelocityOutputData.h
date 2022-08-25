#pragma once

#include "vector.h"
#include "util.h"
#include <chrono>

struct SphereAngles
{
	float theta = 0.0;
	float phi = 0.0;
};

class VelocityOutputData
{
public:
	void CalcVelocity(const Vec3& aLoc, Timestamp_t aNewTimestamp);
	float GetSpeed() const;
	SphereAngles GetDirection() const;
	void TestPrintVelocity() const;
private:
	Vec3 mOldPos = { 0,0,0 };			// {x,y,z}
	//Vec3 mSpeedVec3 = { 0,0,0 };		// {x',y',z'} - Not sure if needed
	float mSpeedMag = 0;				// total speed (magnitude)
	SphereAngles mDirection;					// float theta, float phi
	float mOldTimestampSeconds = 0;
};

