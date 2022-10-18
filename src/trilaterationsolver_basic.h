#pragma once

#include "aabb.h"
#include "vector.h"
#include "trilaterationsolver.h"
#include "velocity_output.h"

// Given the positions of 3 anchors, and their distances to the tag, calculates the 2 possible positions of the tag.
// If more than 3 anchors are provided, the rest will be ignored.
// 
// It is possible for no solution to be found if the 3 anchors are colinear or if the distance values are incorrect.
class TrilaterationSolver_Basic : public ITrilaterationSolver
{
public:
	const char* GetName() const override { return "basic"; }

	TrilaterationResult FindTagPosition( Timestamp_t timestamp, const Vec3* anchorPositions, const float* anchorDistances, size_t numAnchors ) override;

	void SetBounds( const AABB& bounds ) override { mBounds = bounds; }

private:
	bool mHasLastPos;
	Vec3 mLastPos;
	Timestamp_t mLastTimestamp;
	AABB mBounds;
};