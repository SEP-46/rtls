#include "analog_interface.h"
#include <cstdio>

static constexpr uint16_t DAC_MAX_OUTPUT = 4095;
static constexpr uint16_t DAC_MIN_OUTPUT = 0;

#ifdef WIN32
// Microsoft specific code
// ...
AnalogInterface::AnalogInterface(){}
AnalogInterface::~AnalogInterface(){}

bool AnalogInterface::Read(Vec3* data)
{
	return false;
}
bool AnalogInterface::Write(const Vec3& data)
{
	return false;
}
#else
// Linux specific code
// ...
#include <ABE_ADCDACPi.h>

ABElectronics_CPP_Libraries::ADCDACPi gAnalog;

AnalogInterface::AnalogInterface()
{
	setvbuf (stdout, NULL, _IONBF, 0); // needed to print to the command line
	if (gAnalog.open_dac() != 1) // open the DAC spi channel
	{
		printf("DAC failed to open\n");
		return; // if the SPI bus fails to open exit the function
	}
	printf("DAC succesfully opened\n");
	gAnalog.set_dac_gain(2); // set the dac gain to 1 which will give a voltage range of 0 to 2.048V
}

AnalogInterface::~AnalogInterface()
{
	gAnalog.close_dac();
}

bool AnalogInterface::Read(Vec3* data)
{
	return false;
}

bool AnalogInterface::Write(const Vec3& data)
{
	// Sets the voltage to match to location data
	gAnalog.set_dac_raw((uint16_t)Translate(data.x, mMinRange.x, mSpanRange.x), 1); // X location to Ch1
	gAnalog.set_dac_raw((uint16_t)Translate(data.y, mMinRange.y, mSpanRange.y), 2); // Y location to Ch2
	
	return false;
}

#endif

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
float AnalogInterface::Translate(float aValue, float aMinRange, float aSpanRange)
{
	float result = DAC_MIN_OUTPUT + (((aValue - aMinRange) / aSpanRange) * (DAC_MAX_OUTPUT - DAC_MIN_OUTPUT));
	if (result < 0.0)
	{
		return 0.0;
	}
	if (result > 4095.0)
	{
		return 4095.0;
	}
	return result;
}

