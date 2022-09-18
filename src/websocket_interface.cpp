#include "websocket_interface.h"

#include <ixwebsocket/IXWebSocketServer.h>
#include <sstream>

class PImpl
{
public:
	PImpl( int port ) :
		mServer( port )
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

	bool Write( const Vec3& data )
	{
		std::ostringstream ss;
		ss << data.x << ',' << data.y << ',' << data.z;
		std::string data_str = ss.str();

		bool success = true;
		auto clients = mServer.getClients();
		for ( const auto& client : clients )
		{
			ix::WebSocketSendInfo res = client->send( data_str );
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

bool WebSocketInterface::Read( Vec3* data )
{
	return false;
}

bool WebSocketInterface::Write( const Vec3& data )
{
	return mImpl->Write( data );
}
