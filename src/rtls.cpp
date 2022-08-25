#include "rtls.h"

#include "trilaterationsolver_basic.h"
#include <iostream>
#include "uart_interface.h"

RTLS::RTLS()
{
	mTrilaterationSolver = std::make_unique<TrilaterationSolver_Basic>();
}

bool RTLS::Run()
{
	Vec3 pos = { 1.0f, 2.0f, 3.0f };

	UartInterface uart( 0 );
	uart.Write( &pos, sizeof( pos ) );

	if ( !mTag.UpdateDistanceData() )
		return true;

	Vec3 anchorPositions[MAX_ANCHORS];
	float anchorDistances[MAX_ANCHORS];
	size_t numAnchors = mTag.CollectAnchorPositionsAndDistances( anchorPositions, anchorDistances );

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
		std::cout << "Tag possible positions:\n";
		for ( size_t i = 0; i < result.NumPossibleTagPositions(); i++ )
		{
			const Vec3& pos = result.GetPossibleTagPosition( i );
			std::cout << "\t{ " << pos.x << ", " << pos.y << ", " << pos.z << " }\n";
		}
	}

	return true;
}
