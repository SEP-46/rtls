#pragma once

#include "vector.h"
#include <chrono>

class VelocityOutputData
{
public:
	void CalcVelocity(Vec3 aLoc, int32_t aNewTimestamp);
	int32_t GetSpeed();
	int32_t* GetDirection();
	void TestPrintVelocity();
private:
	Vec3 mOldPos = { 0,0,0 };			// {x,y,z}
	Vec3 mDistance = { 0,0,0 };			// {dx,dy,dz}
	//Vec3 mSpeedVec3 = { 0,0,0 };		// {x',y',z'} - Not sure if needed
	int32_t mSpeedMag = 0;				// total speed (magnitude)
	int32_t mDirection[2] = { 0,0 };	// {theta,phi}
	int32_t mOldTimestamp = 0;			// milliseconds?
};

