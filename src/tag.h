#pragma once

#include "util.h"
#include "vector.h"

class Tag
{
public:
	// Reads new distance data from the tag, if available
	// Returns whether any new distance data was available
	virtual bool ReadDistanceData() = 0;

	virtual Timestamp_t GetLastUpdatedTimestamp() const = 0;

	// Gets the latest positions/distances of all connected anchors
	// Returns the number of anchors that we are connected to
	virtual size_t CollectAnchorPositionsAndDistances( Vec3* anchorPositions, float* anchorDistances ) const = 0;
};