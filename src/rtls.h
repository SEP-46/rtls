#pragma once

#include "trilaterationsolver.h"
#include <memory>

class RTLS
{
public:
	RTLS();

	// Run loop, is called indefinitely until it returns false
	bool Run();

private:
	std::unique_ptr<ITrilaterationSolver> mTrilaterationSolver = nullptr;
	Vec3 mAnchors[3];
	Vec3 mTagPosition;
};