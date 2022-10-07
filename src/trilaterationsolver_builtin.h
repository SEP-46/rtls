#pragma once

#include "vector.h"
#include "trilaterationsolver.h"
#include "tag.h"

// Uses the built-in location engine to get the tag position, doesn't actually do any trilateration
class TrilaterationSolver_Builtin : public ITrilaterationSolver
{
public:
	TrilaterationSolver_Builtin( const Tag* tag )
		: mTag( tag )
	{}

	TrilaterationResult FindTagPosition( Timestamp_t timestamp, const Vec3* anchorPositions, const float* anchorDistances, size_t numAnchors ) override
	{
		return TrilaterationResult( mTag->GetBuiltinPosition(), { 0.0f, 0.0f, 0.0f } );
	}

	void SetBounds( const AABB& bounds ) override {}

private:
	const Tag* mTag;
};