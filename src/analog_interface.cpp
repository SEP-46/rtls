#include "analog_interface.h"
#include <cstdio>
#include <iostream>

static constexpr uint16_t DAC_MAX_OUTPUT = 4095;
static constexpr uint16_t DAC_MIN_OUTPUT = 0;

#ifdef WIN32
// Microsoft specific code
// ...
AnalogInterface::AnalogInterface(){}
AnalogInterface::~AnalogInterface(){}

bool AnalogInterface::Write(const Vec3& pos, const Vec3& vel)
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
	if (gAnalog.open_dac() != 1) // open the DAC spi channel
	{
		std::cout << "DAC failed to open\n";
		return; // if the SPI bus fails to open exit the function
	}
	std::cout << "DAC succesfully opened\n";
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

bool AnalogInterface::Write(const Vec3& pos, const Vec3& vel)
{
	// Sets the voltage to match to location data
	gAnalog.set_dac_raw((uint16_t)Translate(pos.x, mBounds.mins.x, mBounds.maxs.x), 1); // X location to Ch1
	gAnalog.set_dac_raw((uint16_t)Translate(pos.y, mBounds.mins.y, mBounds.maxs.y), 2); // Y location to Ch2
	
	return true;
}

#endif

float AnalogInterface::Translate(float aValue, float aMinRange, float aMaxRange)
{
	float result = DAC_MIN_OUTPUT + (((aValue - aMinRange) / (aMaxRange - aMinRange)) * (DAC_MAX_OUTPUT - DAC_MIN_OUTPUT));
	if (result < DAC_MIN_OUTPUT)
	{
		return DAC_MIN_OUTPUT;
	}
	if (result > DAC_MAX_OUTPUT)
	{
		return DAC_MAX_OUTPUT;
	}
	return result;
}

