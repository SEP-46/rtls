#include "velocity_output.h"

#include <iostream>

void VelocityOutputData::CalcVelocity(const Vec3& aLoc, Timestamp_t aNewTimestamp)
{
	// Calculating distance from position data
	Vec3 delta = aLoc - mOldPos;
	float distance = delta.Length();
	mOldPos = aLoc;

	// Timestamp is given in milliseconds, but to calculate speed in m/s we need seconds
	float timestampSeconds = aNewTimestamp / 1000.0f;

	// Speed (total magnitude)
	mSpeedMag = distance / ( timestampSeconds - mOldTimestampSeconds );
	mOldTimestampSeconds = timestampSeconds;

	// Direction {theta, phi}
	float r = distance;
	mDirection.theta = atan2(delta.y, delta.x);
	mDirection.phi = acos(delta.z/r);
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
	std::cout << "\nSpeed: (" << mSpeedMag << ")";
	std::cout << "\nDirection: (" << mDirection.theta << "," << mDirection.phi << ")\n";
}