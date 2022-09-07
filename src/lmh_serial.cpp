#include "lmh_serial.h"

#include <chrono>
#include <serial/serial.h>
#include <thread>

using namespace std::chrono_literals;

static serial::Serial g_serial;

static const char* DEVICE_STRS[] = {
	// For EVB1000
	"STMicroelectronics Virtual COM Port", // STM USB driver
	"USB Serial Device", // Default Windows 10 driver
	"USB2.0 Hub", // Linux

	// For DWM1001
	"CDC",
	"Communications Port",
	"JLink CDC UART Port"
};

/**
 * @brief initializes the LMH utilities over defined interface
 *
 * @param none
 *
 * @return none
 */
static void LMH_Serial_Init()
{
	g_serial.setBaudrate( 115200 );
	g_serial.setBytesize( serial::eightbits );
	g_serial.setParity( serial::parity_none );
	g_serial.setStopbits( serial::stopbits_one );
	g_serial.setFlowcontrol( serial::flowcontrol_none );

	auto timeout = serial::Timeout::simpleTimeout( 100 );
	g_serial.setTimeout( timeout );

	LMH_Log( "Waiting for connection to tag...\n" );

	std::string evb_port;
	while ( evb_port.empty() )
	{
		std::vector<serial::PortInfo> ports = serial::list_ports();

		LMH_Log( "Available ports:\n" );
		for ( size_t i = 0; i < ports.size(); i++ )
		{
			LMH_Log( "\t%i. %s (name=%s, desc=%s, manufacturer=%s, hwid=%s)\n", (int)i,
				ports[i].port.c_str(),
				ports[i].name.c_str(),
				ports[i].description.c_str(),
				ports[i].manufacturer.c_str(),
				ports[i].hardware_id.c_str() );
		}

		for ( const serial::PortInfo& port : ports )
		{
			for ( const char* device_str : DEVICE_STRS )
			{
				if ( port.description == device_str )
				{
					evb_port = port.port;
				}
			}
		}

		if ( evb_port.empty() )
			std::this_thread::sleep_for( 1000ms );
		else
			LMH_Log( "Connected to tag on port %s\n", evb_port.c_str() );
	}

	try
	{
		g_serial.setPort( evb_port );
		g_serial.open();
	}
	catch ( const std::exception& e )
	{
		LMH_Log( "lmh: *****ERROR*****: %s", e.what() );
	}
}


/**
 * @brief de-initializes the LMH utilities over defined interface
 *
 * @param none
 *
 * @return none
 */
static void LMH_Serial_DeInit( void )
{
	g_serial.close();
}

/**
 * @brief transmit data over defined interface
 *
 * @param [in] data: pointer to the Tx data buffer
 * @param [in] length: length of data to be received
 *
 * @return Error code
 */
static int LMH_Serial_Tx( uint8_t* data, uint8_t* length )
{
	size_t tx_length = *length;
	size_t written = g_serial.write( data, tx_length );
	return ( written == tx_length ) ? LMH_OK : LMH_ERR;
}

/**
 * @brief wait for response data over defined interface
 *       note: this function is blocking
 *
 * @param [out] data: pointer to the RX data buffer
 * @param [out] length: length of data to be received
 * @param [in] exp_length: expected length of response data.
 *             Note - If the user doesn't know how long the response from DWM1001 to the host
 *                   is, then this parameter should be set to DWM1001_TLV_MAX_SIZE as defined
 *                   in dwm1001.h. In this case,
 *                   for SPI, length check won't report error no matter how long the received
 *                   data is;
 *                   for UART, this function will not return until the timeout period expires.
 *
 * @return Error code
 */
static int LMH_Serial_WaitForRx( uint8_t* data, uint16_t* length, uint16_t exp_length )
{
	*length = (uint16_t)g_serial.read( data, (size_t)exp_length );
	return LMH_OK;
}

static lmh_impl_t gImpl = {
	LMH_Serial_Init,
	LMH_Serial_DeInit,
	LMH_Serial_Tx,
	LMH_Serial_WaitForRx
};

lmh_impl_t* LMH_Serial_GetImpl()
{
	return &gImpl;
}