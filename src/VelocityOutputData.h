#pragma once

#include "vector.h"
#include <chrono>

class VelocityOutputData
{
public:
	void CalcVelocity(const Vec3 & aLoc, const int64_t & aNewTimestamp);
	float GetSpeed() const;
	float* GetDirection() const;
	void TestPrintVelocity() const;
private:
	Vec3 mOldPos = { 0,0,0 };			// {x,y,z}
	Vec3 mDistance = { 0,0,0 };			// {dx,dy,dz}
	//Vec3 mSpeedVec3 = { 0,0,0 };		// {x',y',z'} - Not sure if needed
	float mSpeedMag = 0;				// total speed (magnitude)
	float mDirection[2] = { 0,0 };		// {theta,phi}
	float mOldTimestamp = 0;			// milliseconds?
};

