#pragma once

class CommunicationInterface
{
public:
	virtual int Read( void* data, int length ) = 0;

	virtual int Write( const void* data, int length = 0 ) = 0;
};