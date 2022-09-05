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
#endif



