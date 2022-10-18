#pragma once

// Includes might be bit confusing here, but vector.h is our own include which contains the Vec3 class
// <vector> is the stdlib include which has the std::vector class (dynamically resizable array)
#include "vector.h"
#include "aabb.h"
#include "util.h"
#include <vector>
#include <cfloat>

struct TrilaterationResult
{
	TrilaterationResult() = default;
	TrilaterationResult( const Vec3& position, const Vec3& velocity ) :
		position( position ), velocity( velocity ), valid( true )
	{}

	Vec3 position;
	Vec3 velocity;
	bool valid = false;
};

class ITrilaterationSolver
{
public:
	virtual const char* GetName() const = 0;

	// Given the positions of anchors, and their distances to the tag, will return the tag location/velocity
	virtual TrilaterationResult FindTagPosition( Timestamp_t timestamp, const Vec3* anchorPositions, const float* anchorDistances, size_t numAnchors ) = 0;

	virtual void SetBounds( const AABB& bounds ) = 0;
};