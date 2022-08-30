#pragma once

#include <cmath>

struct SphereAngles
{
	float theta;
	float phi;
};

inline float Lerp( float a, float b, float t )
{
	return a * ( 1.0f - t ) + b * t;
}

inline bool CloseEnough( float a, float b, float epsilon = 0.001f )
{
	return fabsf( a - b ) < epsilon;
}