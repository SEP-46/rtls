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
}

bool RTLS::Run()
{
	if ( !mTag->ReadDistanceData() )
		return true;

	Vec3 anchorPositions[MAX_ANCHORS];
	float anchorDistances[MAX_ANCHORS];
	size_t numAnchors = mTag->CollectAnchorPositionsAndDistances( anchorPositions, anchorDistances );
	
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
		
		// TESTING ONLY
		// mTestWave.x = 0.5f + 0.5f * cosf( mTestTime );
		// mTestWave.y = 0.5f + 0.5f * sinf( mTestTime );
		// std::cout << "Tag possible positions:\n";
		// std::cout << "\t{ " << mTestWave.x << ", " << mTestWave.y << ", " << mTestWave.z << " }\n";
		// mAnalogInterface.Write( mTestWave );
		// mTestTime += 0.001;

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
