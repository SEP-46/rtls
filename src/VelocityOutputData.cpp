#include "VelocityOutputData.h"

void VelocityOutputData::CalcVelocity(Vec3 aLoc, int32_t aNewTimestamp)
{
	// Calculating distance from position data
	mDistance.x = aLoc.x - mOldPos.x;
	mDistance.y = aLoc.y - mOldPos.y;
	mDistance.z = aLoc.z - mOldPos.z;
	mOldPos.x = aLoc.x;
	mOldPos.y = aLoc.y;
	mOldPos.z = aLoc.z;

	// Speed (total magnitude)
	mSpeedMag = (mDistance / (aNewTimestamp - mOldTimestamp)).Length();
	mOldTimestamp = aNewTimestamp;

	// Direction {theta, phi}
	int32_t r = mDistance.Length();
	mDirection[0] = atan2(mDistance.y, mDistance.x);
	mDirection[1] = acos(mDistance.z/r);
}

int32_t VelocityOutputData::GetSpeed()
{
	return mSpeedMag;
}
int32_t* VelocityOutputData::GetDirection()
{
	return mDirection;
}

void VelocityOutputData::TestPrintVelocity()
{
	printf("\nSpeed: (%i)\n", mSpeedMag);
	printf("Direction: (%i,%i)\n", mDirection[0], mDirection[1]);
}