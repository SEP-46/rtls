#if defined(_WIN32)

/*
 * Copyright (c) 2014 Craig Lilley <cralilley@gmail.com>
 * This software is made available under the terms of the MIT licence.
 * A copy of the licence can be obtained from:
 * http://opensource.org/licenses/MIT
 */

#include "serial/serial.h"
#include <tchar.h>
#include <windows.h>
#include <setupapi.h>
#include <initguid.h>
#include <devguid.h>
#include <cstring>

using serial::PortInfo;
using std::vector;
using std::string;

static const DWORD port_name_max_length = 256;

// Convert a wide Unicode string to an UTF8 string
std::string utf8_encode(const std::wstring &wstr)
{
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo( size_needed, 0 );
	WideCharToMultiByte                  (CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}

std::string device_property( HDEVINFO device_info, SP_DEVINFO_DATA* device_data, DWORD property )
{
	TCHAR buf[256];
	DWORD actual_length = 0;

	BOOL got_property = SetupDiGetDeviceRegistryProperty(
		device_info,
		device_data,
		property,
		NULL,
		(PBYTE)buf,
		256,
		&actual_length );

	if ( got_property == TRUE && actual_length > 0 )
		buf[actual_length - 1] = '\0';
	else
		buf[0] = '\0';

#if UNICODE
	return utf8_encode( buf );
#else
	return buf;
#endif
}

vector<PortInfo>
serial::list_ports()
{
	vector<PortInfo> devices_found;

	HDEVINFO device_info_set = SetupDiGetClassDevs(
		(const GUID *) &GUID_DEVCLASS_PORTS,
		NULL,
		NULL,
		DIGCF_PRESENT);

	unsigned int device_info_set_index = 0;
	SP_DEVINFO_DATA device_info_data;

	device_info_data.cbSize = sizeof(SP_DEVINFO_DATA);

	while(SetupDiEnumDeviceInfo(device_info_set, device_info_set_index, &device_info_data))
	{
		device_info_set_index++;

		// Get port name

		HKEY hkey = SetupDiOpenDevRegKey(
			device_info_set,
			&device_info_data,
			DICS_FLAG_GLOBAL,
			0,
			DIREG_DEV,
			KEY_READ);

		TCHAR port_name[port_name_max_length];
		DWORD port_name_length = port_name_max_length;

		LONG return_code = RegQueryValueEx(
					hkey,
					_T("PortName"),
					NULL,
					NULL,
					(LPBYTE)port_name,
					&port_name_length);

		RegCloseKey(hkey);

		if(return_code != EXIT_SUCCESS)
			continue;

		if(port_name_length > 0 && port_name_length <= port_name_max_length)
			port_name[port_name_length-1] = '\0';
		else
			port_name[0] = '\0';

		// Ignore parallel ports

		if(_tcsstr(port_name, _T("LPT")) != NULL)
			continue;

		// Get port friendly name
		std::string friendlyName = device_property( device_info_set, &device_info_data, SPDRP_FRIENDLYNAME );

		// Get hardware ID
		std::string hardwareId = device_property( device_info_set, &device_info_data, SPDRP_HARDWAREID );

		// Get description
		std::string description = device_property( device_info_set, &device_info_data, SPDRP_DEVICEDESC );

		// Get manufacturer
		std::string manufacturer = device_property( device_info_set, &device_info_data, SPDRP_MFG );

		#ifdef UNICODE
			std::string portName = utf8_encode(port_name);
		#else
			std::string portName = port_name;
		#endif

		PortInfo port_entry;
		port_entry.port = portName;
		port_entry.name = friendlyName;
		port_entry.description = description;
		port_entry.manufacturer = manufacturer;
		port_entry.hardware_id = hardwareId;

		devices_found.push_back(port_entry);
	}

	SetupDiDestroyDeviceInfoList(device_info_set);

	return devices_found;
}

#endif // #if defined(_WIN32)
