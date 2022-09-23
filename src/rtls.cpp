#include "rtls.h"

#include "settings.h"
#include "uwb_tag.h"
#include "mock_tag.h"
#include "trilaterationsolver_basic.h"
#include <iostream>

RTLS::RTLS()
{
	std::cout << "Here 1\n";
	mTrilaterationSolver = std::make_unique<TrilaterationSolver_Basic>();

	const Settings& settings = GetSettings();
	if ( settings.mock_tag )
	{
		std::cout << "Here 2\n";
		mTag = std::make_unique<MockTag>();
	}
	else
	{
		std::cout << "Here 3\n";
		mTag = std::make_unique<UWBTag>();
	}
}

bool RTLS::Run()
{
	std::cout << "Here 4\n";
	// if ( !mTag->ReadDistanceData() )
		// return true;

	Vec3 anchorPositions[MAX_ANCHORS];
	float anchorDistances[MAX_ANCHORS];
	size_t numAnchors = mTag->CollectAnchorPositionsAndDistances( anchorPositions, anchorDistances );
	
	std::cout << "Here 5\n";
	
	TrilaterationResult result = mTrilaterationSolver->FindTagPosition( anchorPositions, anchorDistances, numAnchors );
	if ( !result.SolutionFound() )
	{
		std::cout << "Failed to find tag position!\n";
		std::cout << "Anchors:\n";
		for ( size_t i = 0; i < numAnchors; i++ )
		{
			std::cout << "\t{ " << anchorPositions[i].x << ", " << anchorPositions[i].y << ", " << anchorPositions->z << " }\n";
		}
		std::cout << "Distances:\n";
		for ( size_t i = 0; i < numAnchors; i++ )
		{
			std::cout << "\t" << anchorDistances[i] << "m\n";
		}
	}
	else
	{
		Vec3 bestPos = result.GetPossibleTagPosition( 0 );
		// Vec3 bestPos = { 0.0f, 1.0f, 0.0f };

		std::cout << "Tag possible positions:\n";
		for ( size_t i = 0; i < result.NumPossibleTagPositions(); i++ )
		{
			const Vec3& pos = result.GetPossibleTagPosition( i );
			std::cout << "\t{ " << pos.x << ", " << pos.y << ", " << pos.z << " }\n";
		
			// For now, assume anchors are at z=0
			// Tag is always above the anchors
			if ( pos.z > 0 )
				bestPos = pos;
		}

		mVelocityOutputData.CalcVelocity( bestPos, Util_GetCurrentTime() );
		mVelocityOutputData.TestPrintVelocity();

		// TODO: May need to be byteswapped, or write in text format
		mUartInterface.Write( bestPos );
		mAnalogInterface.Write( bestPos );
		mWebSocketInterface.Write( bestPos );
	}

	return true;
}
