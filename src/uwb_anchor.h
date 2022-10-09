#pragma once

#include "node.h"
#include "util.h"
#include "vector.h"
#include <cassert>
#include <cstdint>

// The number here comes from DWM API
constexpr int MAX_ANCHORS = 14;

class UWBAnchor
{
public:
	UWBAnchor() = default;
	UWBAnchor( NodeId_t id ) :
		mId( id )
	{}

	NodeId_t GetId() const { return mId; }

	const Vec3& GetPosition() const { return mPosition; }
	void SetPosition( const Vec3& position ) { mPosition = position; }

	float GetDistance() const { return mDistance; }
	void SetDistance( float distance ) { mDistance = distance; }

	float GetDistanceQualityFactor() const { return mDistanceQualityFactor; }
	void SetDistanceQualityFactor( float distanceQualityFactor ) { mDistanceQualityFactor = distanceQualityFactor; }

	Timestamp_t GetLastUpdatedTimestamp() const { return mLastUpdateTimestamp; }
	void SetLastUpdatedTimestamp( Timestamp_t timestamp ) { mLastUpdateTimestamp = timestamp; }

private:
	NodeId_t mId = NODE_ID_INVALID;
	Vec3 mPosition = { 0.0f, 0.0f, 0.0f };
	float mDistance = 0.0f;
	float mDistanceQualityFactor = 0.0f;
	Timestamp_t mLastUpdateTimestamp = 0;
};

// Helper class to keep track of connected anchors
class UWBAnchorList
{
public:
	size_t NumAnchors() const { return mNumAnchors; };

	UWBAnchor& GetAnchorByIndex( size_t index ) { return mAnchors[index]; }
	const UWBAnchor& GetAnchorByIndex( size_t index ) const { return mAnchors[index]; }

	UWBAnchor& AddAnchor( NodeId_t id );
	void RemoveAnchorByIndex( size_t index );

	int FindAnchorIndexById( NodeId_t id ) const;
	const UWBAnchor* FindAnchorById( NodeId_t id ) const;

private:
	UWBAnchor mAnchors[MAX_ANCHORS];
	size_t mNumAnchors = 0;
};