#include <lmh.h>

//
// LMH is the Low-level Module Handshake API, it allows communicating with the DWM1001
// over an abstract interface. In the original examples, this was implemented using
// the Raspberry Pi HAL API, which isn't very portable.
// 
// This file instead implements the LMH interface by using the serial library, which
// works on various different platforms, including Windows/Linux.
//

#include <serial/serial.h>

static serial::Serial g_serial;

static constexpr char* DEVICE_STRS[] = {
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
void LMH_Init()
{
	g_serial.setBaudrate( 115200 );
	g_serial.setBytesize( serial::eightbits );
	g_serial.setParity( serial::parity_none );
	g_serial.setStopbits( serial::stopbits_one );
	g_serial.setFlowcontrol( serial::flowcontrol_none );

	auto timeout = serial::Timeout::simpleTimeout( 1000 );
	g_serial.setTimeout( timeout );

	std::string evb_port;
	while ( evb_port.empty() )
	{
		std::vector<serial::PortInfo> ports = serial::list_ports();
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
void LMH_DeInit( void )
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
int  LMH_Tx( uint8_t* data, uint8_t* length )
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
int  LMH_WaitForRx( uint8_t* data, uint16_t* length, uint16_t exp_length )
{
	*length = (uint16_t)g_serial.read( data, (size_t)exp_length );
	return LMH_OK;
}