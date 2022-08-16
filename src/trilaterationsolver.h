#pragma once

// Includes might be bit confusing here, but vector.h is our own include which contains the Vec3 class
// <vector> is the stdlib include which has the std::vector class (dynamically resizable array)
#include "vector.h"
#include <vector>

class TrilaterationResult
{
public:
	TrilaterationResult() = default;
	TrilaterationResult( std::vector<Vec3> possibleTagPositions ) :
		mPossibleTagPositions( std::move( possibleTagPositions ) )
	{}

	bool SolutionFound() const { return !mPossibleTagPositions.empty(); }
	size_t NumPossibleTagPositions() const { return mPossibleTagPositions.size(); }
	const Vec3& GetPossibleTagPosition( size_t index ) { return mPossibleTagPositions[index]; }

private:
	std::vector<Vec3> mPossibleTagPositions;
};

class ITrilaterationSolver
{
public:
	// Given the positions of anchors, and their distances to the tag, will return possible locations the tag can be in
	virtual TrilaterationResult FindTagPosition( const Vec3* anchorPositions, const float* anchorDistances, size_t numAnchors ) = 0;
};