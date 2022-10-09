#pragma once

#include "communication_interface.h"
#include <memory>

class WebSocketInterface : public CommunicationInterface
{
public:
	WebSocketInterface( int port );
	~WebSocketInterface();

	bool Write( const Vec3& pos, const Vec3& vel ) override;

private:
	std::unique_ptr<class PImpl> mImpl;
};
