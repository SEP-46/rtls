#include "analog_interface.h"

#ifdef WIN32
// Microsoft specific code
// ...
AnalogInterface::AnalogInterface()
{

}

int AnalogInterface::Read(Vec3* data)
{
	return 0;
}

int AnalogInterface::Write(const Vec3& data)
{
	return 0;
}
#else
// Linux specific code
// ...
#include <ABE_ADCDACPi.h>

static ABElectronics_CPP_Libraries::ADCDACPi gAnalog;

AnalogInterface::AnalogInterface()
{
	if (gAnalog.open_dac() != 1) // open the DAC spi channel
	{
		return(1); // if the SPI bus fails to open exit the program
	}
	adcdac.set_dac_gain(2); // set the dac gain to 2 which will give a voltage range of 0 to 3.3V
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
	// Sets the voltage to match to location data in millimeters
	gAnalog.set_dac_raw(uint16_t (1000*data.x), uint8_t 1); // X location to Ch1 (converts meters to mm)
	gAnalog.set_dac_raw(uint16_t (1000*data.y), uint8_t 2); // Y location to Ch2 (converts meters to mm)
	return 0;
}
#endif



