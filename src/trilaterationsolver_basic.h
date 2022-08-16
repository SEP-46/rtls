#pragma once

#include "trilaterationsolver.h"

// Given the positions of 3 anchors, and their distances to the tag, calculates the 2 possible positions of the tag.
// If more than 3 anchors are provided, the rest will be ignored.
// 
// It is possible for no solution to be found if the 3 anchors are colinear or if the distance values are incorrect.
class TrilaterationSolver_Basic : public ITrilaterationSolver
{
public:
	virtual TrilaterationResult FindTagPosition( const Vec3* anchorPositions, const float* anchorDistances, size_t numAnchors ) override;
};