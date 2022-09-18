#pragma once

#include "communication_interface.h"
#include <memory>

class WebSocketInterface : public CommunicationInterface
{
public:
	WebSocketInterface( int port );
	~WebSocketInterface();

	virtual bool Read( Vec3* data ) override;
	virtual bool Write( const Vec3& data ) override;

private:
	std::unique_ptr<class PImpl> mImpl;
};
