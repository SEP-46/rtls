#pragma once

#include "trilaterationsolver.h"
#include "uwb_tag.h"
#include <memory>

class RTLS
{
public:
	RTLS();

	// Run loop, is called indefinitely until it returns false
	bool Run();

private:
	UWBTag mTag;
	std::unique_ptr<ITrilaterationSolver> mTrilaterationSolver = nullptr;
};