#include "rtls.h"

#include "trilaterationsolver_basic.h"
#include <iostream>

RTLS::RTLS()
{
	mTrilaterationSolver = std::make_unique<TrilaterationSolver_Basic>();

	// Create 3 anchors, 1 along each axis
	mAnchors[0] = { 1.0f, 0.0f, 0.0f };
	mAnchors[1] = { 0.0f, 1.0f, 0.0f };
	mAnchors[2] = { 0.0f, 0.0f, 1.0f };

	// Start with tag at origin
	mTagPosition = { 0.0f, 0.0f, 0.0f };
}

bool RTLS::Run()
{
	const float dt = 1.0f / 60.0f; // Delta time, advance by 1/60th of a second

	// Move tag in circular path
	mTagPosition.x = sinf( dt );
	mTagPosition.y = cosf( dt );

	float distances[3];
	for ( int i = 0; i < 3; i++ )
	{
		distances[i] = Vec3::Distance( mAnchors[i], mTagPosition );
	}

	TrilaterationResult result = mTrilaterationSolver->FindTagPosition( mAnchors, distances, 3 );

	const Vec3* solution = result.FindClosestSolution( mTagPosition );
	if ( !solution || !Vec3::CloseEnough( *solution, mTagPosition ) )
	{
		std::cerr << "Trilateration solver failed to find tag location!\n";
		return false;
	}

	return true;
}
