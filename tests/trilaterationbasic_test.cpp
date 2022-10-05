#include <gtest/gtest.h>

#include "trilaterationsolver_basic.h"

TEST( TrilaterationBasic, BasicCase )
{
	Vec3 anchors[] = {
		Vec3( 0.0f, 0.0f, 0.0f ),
		Vec3( 0.5f, 0.0f, 0.0f ),
		Vec3( 0.0f, 0.5f, 0.0f )
	};

	// Distances from above anchors to a tag located at (0.5, 0.5)
	float distances[] = {
		sqrtf( 0.5f ),
		0.5f,
		0.5f
	};

	TrilaterationSolver_Basic solver;
	TrilaterationResult result = solver.FindTagPosition( Util_GetCurrentTime(), anchors, distances, 3 );
	EXPECT_TRUE( Vec3::CloseEnough( result.position, Vec3( 0.5f, 0.5f, 0.0f ) ) );
}

TEST( TrilaterationBasic, NonIntersecting )
{
	Vec3 anchors[] = {
		Vec3( 0.0f, 0.0f, 0.0f ),
		Vec3( 0.5f, 0.0f, 0.0f ),
		Vec3( 0.0f, 0.5f, 0.0f )
	};

	float distances[] = {
		0.174f,
		0.23f,
		0.188f
	};

	TrilaterationSolver_Basic solver;
	TrilaterationResult result = solver.FindTagPosition( Util_GetCurrentTime(), anchors, distances, 3 );
	EXPECT_TRUE( result.valid );
}

TEST( TrilaterationBasic, Colinear )
{
	Vec3 anchors[] = {
		Vec3( 0.0f, 0.0f, 0.0f ),
		Vec3( 0.5f, 0.0f, 0.0f ),
		Vec3( 1.0f, 0.0f, 0.0f )
	};

	// Distances from above anchors to a tag located at (0.5, 0.5)
	float distances[] = {
		sqrtf( 0.5f ),
		0.5f,
		sqrtf( 0.5f )
	};

	TrilaterationSolver_Basic solver;
	TrilaterationResult result = solver.FindTagPosition( Util_GetCurrentTime(), anchors, distances, 3 );
	EXPECT_FALSE( result.valid );
}