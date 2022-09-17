#include "analog_interface.h"
#include <cmath>

#define ResCE 244.2002442 // Resolution Coefficent (1000/4.095)

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
void AnalogInterface::SetRange(Vec3 aMin, Vec3 aMax)
{
	mMinRange = aMin;
	mMaxRange = aMax;
	SetResolution();
}
void AnalogInterface::SetResolution()
{
	Vec3 MagnitudeRange = mMaxRange - mMinRange;
	mResolution.x = abs(MagnitudeRange.x)*ResCE;
	mResolution.y = abs(MagnitudeRange.y)*ResCE;
	mResolution.z = abs(MagnitudeRange.z)*ResCE;
}

uint16_t AnalogInterface::CastAsInt(float aResAxis, float aTagCoord)
{
	float result = aResAxis * aTagCoord;
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
	gAnalog.set_dac_raw(CastAsInt(mResolution.x, data.x), 1); // X location to Ch1
	gAnalog.set_dac_raw(CastAsInt(mResolution.y, data.y), 2); // Y location to Ch2
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
void AnalogInterface::SetRange(Vec3 aMin, Vec3 aMax)
{
	mMinRange = aMin;
	mMaxRange = aMax;
	SetResolution();
}
void AnalogInterface::SetResolution()
{
	Vec3 MagnitudeRange = mMaxRange - mMinRange;
	mResolution.x = abs(MagnitudeRange.x) * ResCE;
	mResolution.y = abs(MagnitudeRange.y) * ResCE;
	mResolution.z = abs(MagnitudeRange.z) * ResCE;
}

uint16_t AnalogInterface::CastAsInt(float aResAxis, float aTagCoord)
{
	float result = aResAxis * aTagCoord;
	return (uint16_t)result;
}

#endif



