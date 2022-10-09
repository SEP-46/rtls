#pragma once

#include "node.h"
#include "uwb_anchor.h"
#include "util.h"
#include "vector.h"
#include <vector>

struct AnchorDistanceMeasurement
{
	NodeId_t id;
	float distance;
};

class Tag
{
public:
	// Gets the node ID of the connected tag
	virtual NodeId_t GetId() const = 0;

	// Reads new distance data from the tag, if available
	// Returns whether any new distance data was available
	virtual bool ReadDistanceData() = 0;

	virtual Timestamp_t GetLastUpdatedTimestamp() const = 0;

	// Gets the latest positions/distances of all connected anchors
	virtual std::vector<AnchorDistanceMeasurement> CollectAnchorDistances() const = 0;

	virtual const UWBAnchorList& GetAnchorList() const = 0;

	// Gets the position of the tag using the MDEK1001 built-in location engine
	virtual Vec3 GetBuiltinPosition() const = 0;
};