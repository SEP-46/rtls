#include <gtest/gtest.h>
#include "analog_interface.h"

TEST(AnalogTest, CanGetMinAndMaxRange)
{
	AnalogInterface testAnalog;
	Vec3 testRangeMin = { 0.0f, 0.0f, 0.0f };
	Vec3 testRangeMax = { 4.2f, 4.0f, 4.1f };
	testAnalog.SetRange(testRangeMin, testRangeMax);

	EXPECT_FLOAT_EQ(testAnalog.GetMinRange().x, 0.0f);
	EXPECT_FLOAT_EQ(testAnalog.GetMinRange().y, 0.0f);
	EXPECT_FLOAT_EQ(testAnalog.GetMinRange().z, 0.0f);

	EXPECT_FLOAT_EQ(testAnalog.GetMaxRange().x, 4.2f);
	EXPECT_FLOAT_EQ(testAnalog.GetMaxRange().y, 4.0f);
	EXPECT_FLOAT_EQ(testAnalog.GetMaxRange().z, 4.1f);
}

TEST(AnalogTest, CanTranslate)
{
	AnalogInterface testAnalog;
	Vec3 testRangeMin = { 0.0f, 0.0f, 0.0f };
	Vec3 testRangeMax = { 4.2f, 4.0f, 4.1f };
	float value = 0.5f;
	testAnalog.SetRange(testRangeMin, testRangeMax);

	EXPECT_FLOAT_EQ(testAnalog.Translate(value, testRangeMin.x, testRangeMax.x), 487.5);
	EXPECT_FLOAT_EQ(testAnalog.Translate(value, testRangeMin.y, testRangeMax.y), 511.875);
	EXPECT_FLOAT_EQ(testAnalog.Translate(value, testRangeMin.z, testRangeMax.z), 499.3902439024);
}
