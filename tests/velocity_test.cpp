#include <gtest/gtest.h>
#include "VelocityOutputData.h"

TEST(VelocityTest, CanGetSpeed)
{
	VelocityOutputData testOutputData;
	Vec3 testLoc1 = { 1.2f, 2.4f, 3.6f };
	Vec3 testLoc2 = { 6.3f, 4.2f, 2.1f };
	Timestamp_t testTime1 = 4321;
	Timestamp_t testTime2 = 8642;

	testOutputData.CalcVelocity(testLoc1, testTime1);
	testOutputData.CalcVelocity(testLoc2, testTime2);

	EXPECT_FLOAT_EQ(testOutputData.GetSpeed(),(sqrtf(powf(6.3f-1.2f,2)+powf(4.2f - 2.4f, 2)+powf(2.1f - 3.6f, 2)))/(8.642f-4.321f));
}

TEST(VelocityTest, CanGetirection)
{
	VelocityOutputData testOutputData;
	Vec3 testLoc1 = { 1.2f, 2.4f, 3.6f };
	Timestamp_t testTime1 = 4321;

	testOutputData.CalcVelocity(testLoc1, testTime1);



	EXPECT_FLOAT_EQ(testOutputData.GetDirection().theta, atan2(2.4f, 1.2f));
	EXPECT_FLOAT_EQ(testOutputData.GetDirection().phi, acos(3.6f / sqrtf(powf(1.2f, 2) + powf(2.4f, 2) + powf(3.6f, 2))));
}
