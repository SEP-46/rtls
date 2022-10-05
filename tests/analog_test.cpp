#include <gtest/gtest.h>
#include "analog_interface.h"

TEST(AnalogTest, CanTranslate)
{
	AnalogInterface testAnalog;
	Vec3 testRangeMin = { 0.0f, 0.0f, 0.0f };
	Vec3 testRangeMax = { 4.2f, 4.0f, 4.1f };
	AABB bounds(testRangeMin, testRangeMax);
	testAnalog.SetBounds(bounds);

	float value = 0.5f;

	EXPECT_FLOAT_EQ(testAnalog.Translate(value, testRangeMin.x, testRangeMax.x), 487.5f);
	EXPECT_FLOAT_EQ(testAnalog.Translate(value, testRangeMin.y, testRangeMax.y), 511.875f);
	EXPECT_FLOAT_EQ(testAnalog.Translate(value, testRangeMin.z, testRangeMax.z), 499.3902439024f);
}
