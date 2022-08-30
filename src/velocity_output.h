#pragma once

#include "vector.h"
#include "util.h"
#include "mathlib.h"
#include <chrono>

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
	SphereAngles mDirection = { 0.0f, 0.0f };					// float theta, float phi
	float mOldTimestampSeconds = 0;
};

