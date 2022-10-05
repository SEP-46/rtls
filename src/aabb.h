#pragma once

#include "vector.h"

// Axis-aligned bounding box
struct AABB
{
	AABB() = default;
	AABB( const Vec3& mins, const Vec3& maxs ) :
		mins( mins ), maxs( maxs )
	{}

	bool Contains( const Vec3& point )
	{
		return point.x >= mins.x && point.x <= maxs.x &&
			point.y >= mins.y && point.y <= maxs.y &&
			point.z >= mins.z && point.z <= maxs.z;
	}

	Vec3 mins;
	Vec3 maxs;
};