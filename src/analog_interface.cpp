#include "analog_interface.h"

static constexpr uint16_t DAC_MAX_OUTPUT = 4095;
static constexpr uint16_t DAC_MIN_OUTPUT = 0;

#ifdef WIN32
// Microsoft specific code
// ...
AnalogInterface::AnalogInterface()
{
	return;
}

int AnalogInterface::Read(Vec3* data)
{
	return 0;
}
int AnalogInterface::Write(const Vec3& data)
{
	return 0;
}

Vec3 AnalogInterface::GetMinRange()
{
	return mMinRange;
}
Vec3 AnalogInterface::GetMaxRange()
{
	return mMaxRange;
}
void AnalogInterface::SetRange(const Vec3& aMin, const Vec3& aMax)
{
	mMinRange = aMin;
	mMaxRange = aMax;
	mSpanRange = mMaxRange - mMinRange;
}
uint16_t AnalogInterface::Translate(float aValue, float aMinRange, float aSpanRange)
{
	float result = DAC_MIN_OUTPUT + ((aValue - aMinRange / aSpanRange) * (DAC_MAX_OUTPUT - DAC_MIN_OUTPUT));
	return (uint16_t)result;
}

#else
// Linux specific code
// ...
#include <ABE_ADCDACPi.h>

static ABElectronics_CPP_Libraries::ADCDACPi gAnalog;

AnalogInterface::AnalogInterface()
{
	if (!gAnalog.open_dac()) // open the DAC spi channel
	{
		return; // if the SPI bus fails to open exit the function
	}
	adcdac.set_dac_gain(1); // set the dac gain to 1 which will give a voltage range of 0 to 2.048V
}

~AnalogInterface::AnalogInterface()
{
	gAnalog.close_dac();
}

int AnalogInterface::Read(Vec3* data)
{
	return 0;
}

int AnalogInterface::Write(const Vec3& data)
{
	// Sets the voltage to match to location data
	gAnalog.set_dac_raw(Translate(data.x, mMinRange.x, mSpanRange.x), 1); // X location to Ch1
	gAnalog.set_dac_raw(Translate(data.y, mMinRange.y, mSpanRange.y), 2); // Y location to Ch2
	return 0;
}

Vec3 AnalogInterface::GetMinRange()
{
	return mMinRange;
}
Vec3 AnalogInterface::GetMaxRange()
{
	return mMaxRange;
}
void AnalogInterface::SetRange(const Vec3& aMin, const Vec3& aMax)
{
	mMinRange = aMin;
	mMaxRange = aMax;
	mSpanRange = mMaxRange - mMinRange;
}
uint16_t AnalogInterface::Translate(float aValue, float aMinRange, float aSpanRange)
{
	float result = DAC_MIN_OUTPUT + ((aValue - aMinRange / aSpanRange) * (DAC_MAX_OUTPUT - DAC_MIN_OUTPUT));
	return (uint16_t)result;
}

#endif



