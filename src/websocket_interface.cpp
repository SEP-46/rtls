#include "websocket_interface.h"

#include <ixwebsocket/IXWebSocketServer.h>
#include <json.hpp>
#include <sstream>

#include <iostream>

using namespace nlohmann;

class PImpl
{
public:
	PImpl( int port ) :
		mServer( port, "0.0.0.0" )
	{
		mServer.setOnClientMessageCallback(
			[]( std::shared_ptr<ix::ConnectionState> connectionState, ix::WebSocket& webSocket, const ix::WebSocketMessagePtr& msg ) {} );

		auto res = mServer.listen();
		if ( !res.first )
		{
			return;
		}

		// Per message deflate connection is enabled by default. It can be disabled
		// which might be helpful when running on low power devices such as a Rasberry Pi
		mServer.disablePerMessageDeflate();
		
		// Run the server in the background. Server can be stoped by calling server.stop()
		mServer.start();
	}

	bool Write( const Vec3& pos, const Vec3& vel )
	{
		json data;

		json posJson;
		posJson["x"] = pos.x;
		posJson["y"] = pos.y;
		posJson["z"] = pos.z;
		data["pos"] = posJson;

		json velJson;
		velJson["x"] = vel.x;
		velJson["y"] = vel.y;
		velJson["z"] = vel.z;
		data["vel"] = velJson;

		bool success = true;
		auto clients = mServer.getClients();
		for ( const auto& client : clients )
		{
			ix::WebSocketSendInfo res = client->send( data.dump() );
			success &= res.success;
		}

		return success;
	}

private:
	ix::WebSocketServer mServer;
};

WebSocketInterface::WebSocketInterface( int port )
{
	ix::initNetSystem();

	mImpl = std::make_unique<PImpl>( port );
	
}

WebSocketInterface::~WebSocketInterface()
{
	ix::uninitNetSystem();
}

bool WebSocketInterface::Write( const Vec3& pos, const Vec3& vel )
{
	return mImpl->Write( pos, vel );
}
