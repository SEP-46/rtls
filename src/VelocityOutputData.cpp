#include "VelocityOutputData.h"

void VelocityOutputData::CalcVelocity(const Vec3& aLoc, int64_t aNewTimestamp)
{
	// Calculating distance from position data
	mDistance = aLoc - mOldPos;
	mOldPos = aLoc;

	// Speed (total magnitude)
	mSpeedMag = (mDistance / (aNewTimestamp - mOldTimestamp)).Length();
	mOldTimestamp = aNewTimestamp;

	// Direction {theta, phi}
	float r = mDistance.Length();
	mDirection.theta = atan2(mDistance.y, mDistance.x);
	mDirection.phi = acos(mDistance.z/r);
}

float VelocityOutputData::GetSpeed() const
{
	return mSpeedMag;
}
SphereAngles VelocityOutputData::GetDirection() const
{
	return mDirection;
}

void VelocityOutputData::TestPrintVelocity() const
{
	printf("\nSpeed: (%f)\n", mSpeedMag);
	printf("Direction: (%f,%f)\n", mDirection.theta, mDirection.phi);
}