#pragma once

#include "analog_interface.h"
#include "tag.h"
#include "trilaterationsolver.h"
#include "uart_interface.h"
#include "velocity_output.h"
#include "websocket_interface.h"
#include <memory>

class RTLS
{
public:
	RTLS();

	// Run loop, is called indefinitely until it returns false
	bool Run();

private:
	std::unique_ptr<Tag> mTag;
	std::unique_ptr<ITrilaterationSolver> mTrilaterationSolver = nullptr;
	UartInterface mUartInterface = UartInterface( 1 );
	AnalogInterface mAnalogInterface = AnalogInterface();
	WebSocketInterface mWebSocketInterface = WebSocketInterface( 9002 );
	VelocityOutputData mVelocityOutputData;
	
	// TESTING ONLY
	// Vec3 mTestWave = { 0.0f, 0.0f, 0.0f };
	// float mTestTime = 0.0f;
};