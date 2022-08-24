#include "VelocityOutputData.h"

void VelocityOutputData::CalcVelocity(const Vec3 & aLoc, const int64_t & aNewTimestamp)
{
	// Calculating distance from position data
	mDistance = aLoc - mOldPos;
	mOldPos = aLoc;

	// Speed (total magnitude)
	mSpeedMag = (mDistance / (aNewTimestamp - mOldTimestamp)).Length();
	mOldTimestamp = aNewTimestamp;

	// Direction {theta, phi}
	float r = mDistance.Length();
	mDirection[0] = atan2(mDistance.y, mDistance.x);
	mDirection[1] = acos(mDistance.z/r);
}

float VelocityOutputData::GetSpeed() const
{
	return mSpeedMag;
}
float* VelocityOutputData::GetDirection() const
{
	return mDirection;
}

void VelocityOutputData::TestPrintVelocity() const
{
	printf("\nSpeed: (%i)\n", mSpeedMag);
	printf("Direction: (%i,%i)\n", mDirection[0], mDirection[1]);
}