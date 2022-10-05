#include "rtls.h"

#include "settings.h"
#include "uwb_tag.h"
#include "mock_tag.h"
#include "trilaterationsolver_basic.h"
#include <iostream>

RTLS::RTLS()
{
	mTrilaterationSolver = std::make_unique<TrilaterationSolver_Basic>();

	const Settings& settings = GetSettings();
	if ( settings.mock_tag )
	{
		mTag = std::make_unique<MockTag>();
	}
	else
	{
		mTag = std::make_unique<UWBTag>();
	}

	SetBounds( AABB( { 0.0f, 0.0f, 0.0f }, { 10.0f, 10.0f, 10.0f } ) );
}

bool RTLS::Run()
{
	if ( !mTag->ReadDistanceData() )
		return true;

	Vec3 anchorPositions[MAX_ANCHORS];
	float anchorDistances[MAX_ANCHORS];
	size_t numAnchors = mTag->CollectAnchorPositionsAndDistances( anchorPositions, anchorDistances );
	Timestamp_t timestamp = mTag->GetLastUpdatedTimestamp();

	TrilaterationResult result = mTrilaterationSolver->FindTagPosition( timestamp, anchorPositions, anchorDistances, numAnchors );
	
	std::cout << "Tag position: " << result.position.x << ", " << result.position.y << ", " << result.position.z << '\n';
	std::cout << "Tag velocity: " << result.velocity.x << ", " << result.velocity.y << ", " << result.velocity.z << '\n';

	const Vec3& pos = result.position;

	mVelocityOutputData.CalcVelocity( pos, timestamp );
	mVelocityOutputData.TestPrintVelocity();

	// TODO: May need to be byteswapped, or write in text format
	mUartInterface.Write( pos );
	mAnalogInterface.Write( pos );
	mWebSocketInterface.Write( pos );

	return true;
}

std::vector<Vec3> RTLS::GetAnchorPos()
{
	return {
		Vec3(1, 2, 3),
		Vec3(4, 5, 6),
		Vec3(7, 8, 9),
	};
}

void RTLS::SetBounds( const AABB& bounds )
{
	mTrilaterationSolver->SetBounds( bounds );
	mAnalogInterface.SetBounds( bounds );
}
