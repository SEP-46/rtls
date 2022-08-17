#pragma once

#include "vector.h"
#include <cassert>
#include <cstdint>

// The number here comes from DWM API
constexpr int MAX_ANCHORS = 14;

// A unique ID given to each anchor
// Although you can use an anchors index to interact with it, the index is ephemeral
// and may not be stable as the anchors connect/disconnect to the tag
using UWBAnchorId_t = uint16_t;
constexpr UWBAnchorId_t UWB_ANCHOR_ID_INVALID = 0;

class UWBAnchor
{
public:
	UWBAnchor() = default;
	UWBAnchor( UWBAnchorId_t id ) :
		mId( id )
	{}

	UWBAnchorId_t GetId() const { return mId; }

	const Vec3& GetPosition() const { return mPosition; }
	void SetPosition( const Vec3& position ) { mPosition = position; }

	float GetDistance() const { return mDistance; }
	void SetDistance( float distance ) { mDistance = distance; }

	float GetDistanceQualityFactor() const { return mDistanceQualityFactor; }
	void SetDistanceQualityFactor( float distanceQualityFactor ) { mDistanceQualityFactor = distanceQualityFactor; }

	int64_t GetLastUpdatedTimestamp() const { return mLastUpdateTimestamp; }
	void SetLastUpdatedTimestamp( int64_t timestamp ) { mLastUpdateTimestamp = timestamp; }

private:
	UWBAnchorId_t mId = UWB_ANCHOR_ID_INVALID;
	Vec3 mPosition = { 0.0f, 0.0f, 0.0f };
	float mDistance = 0.0f;
	float mDistanceQualityFactor = 0.0f;
	int64_t mLastUpdateTimestamp = 0;
};

// Helper class to keep track of connected anchors
class UWBAnchorList
{
public:
	size_t NumAnchors() const { return mNumAnchors; };

	UWBAnchor& GetAnchorByIndex( size_t index ) { return mAnchors[index]; }
	const UWBAnchor& GetAnchorByIndex( size_t index ) const { return mAnchors[index]; }

	UWBAnchor& AddAnchor( UWBAnchorId_t id );
	void RemoveAnchorByIndex( size_t index );

	int FindAnchorIndexById( UWBAnchorId_t id );
	UWBAnchor* FindAnchorById( UWBAnchorId_t id );

private:
	UWBAnchor mAnchors[MAX_ANCHORS];
	size_t mNumAnchors = 0;
};