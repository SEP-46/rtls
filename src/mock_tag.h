#pragma once

#include "tag.h"
#include "uwb_anchor.h"
#include <cstdint>

// Class that simulates a moving tag, so that code can be tested without having actual kit connected
// Currently just simulates the tag moving in a fixed circle on xy plane
class MockTag : public Tag
{
public:
	MockTag();

	NodeId_t GetId() const override { return (NodeId_t)-1; }

	// Reads new distance data from the anchors, if available
	// Returns whether any new distance data has been read from the anchors
	bool ReadDistanceData() override;

	virtual Timestamp_t GetLastUpdatedTimestamp() const override { return mLastUpdate; };

	// Gets the latest positions/distances of all connected anchors
	// Returns the number of anchors that we are connected to
	std::vector<AnchorDistanceMeasurement> CollectAnchorDistances() const override;

	const UWBAnchorList& GetAnchorList() const override { return mAnchorList; }

	// Gets the position of the tag using the MDEK1001 built-in location engine
	Vec3 GetBuiltinPosition() const override;

private:
	Timestamp_t mLastUpdate = 0;
	UWBAnchorList mAnchorList;
	Vec3 mTagPosition = { 0.5f, 0.5f, 0.0f };
};