#pragma once

#include "uwb_anchor.h"
#include <cstdint>

// Handled communication with UWB tag device
class UWBTag
{
public:
	UWBTag();
	~UWBTag();

	// Reads new distance data from the anchors, if available
	// Returns whether any new distance data has been read from the anchors
	bool UpdateDistanceData();

	// Gets the latest positions/distances of all connected anchors
	// Returns the number of anchors that we are connected to
	size_t CollectAnchorPositionsAndDistances( Vec3* anchorPositions, float* anchorDistances ) const;

private:
	UWBAnchorList mAnchorList;
};