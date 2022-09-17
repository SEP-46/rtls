#pragma once

#include "tag.h"
#include "trilaterationsolver.h"
#include "uart_interface.h"
#include "velocity_output.h"
#include "websocket_interface.h"
#include <memory>
#include <vector>

class RTLS
{
public:
	RTLS();

	// Run loop, is called indefinitely until it returns false
	bool Run();

	std::vector<Vec3> GetAnchorPos();

private:
	std::unique_ptr<Tag> mTag;
	std::unique_ptr<ITrilaterationSolver> mTrilaterationSolver = nullptr;
	UartInterface mUartInterface = UartInterface( 1 );
	WebSocketInterface mWebSocketInterface = WebSocketInterface( 9002 );
	VelocityOutputData mVelocityOutputData;
};