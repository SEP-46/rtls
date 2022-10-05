#pragma once

#include "aabb.h"
#include "analog_interface.h"
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
	void SetAnchorPos(int AnchorIndex, Vec3 position) {}
	
	void SetBounds( const AABB& bounds );
	const AABB& GetBounds() { return mBounds; }

private:
	std::unique_ptr<Tag> mTag;
	std::unique_ptr<ITrilaterationSolver> mTrilaterationSolver = nullptr;
	UartInterface mUartInterface = UartInterface( 1 );
	AnalogInterface mAnalogInterface = AnalogInterface();
	WebSocketInterface mWebSocketInterface = WebSocketInterface( 9002 );
	VelocityOutputData mVelocityOutputData;
	AABB mBounds;
};