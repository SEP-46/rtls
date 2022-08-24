#include "trilaterationsolver_basic.h"

#include <cmath>

TrilaterationResult TrilaterationSolver_Basic::FindTagPosition(
	const Vec3* anchorPositions,
	const float* anchorDistances,
	size_t numAnchors )
{
	if ( numAnchors < 3 )
		return {};

	const Vec3* P = anchorPositions;
	const float* D = anchorDistances;

	// Find the 2 planes that sit at intersection of the 3 spheres
	Vec3 plane1_normal = P[1] - P[0];
	float plane1_dist = 0.5f * ( D[0]*D[0] - D[1]*D[1] - P[0].LengthSq() + P[1].LengthSq() );
	Vec3 plane2_normal = P[2] - P[0];
	float plane2_dist = 0.5f * ( D[0]*D[0] - D[2]*D[2] - P[0].LengthSq() + P[2].LengthSq() );

	// Find paramters of line at intersection of the 2 planes

	// First parameter is direction of line
	Vec3 Ld = Vec3::Cross( plane1_normal, plane2_normal );
	if ( Ld.LengthSq() == 0 ) // Can happen if the 2 planes are parallel (ie. the 3 spheres are colinear)
		return {};

	Vec3 Lp = ( Vec3::Cross( Ld, plane1_normal ) * plane2_dist + Vec3::Cross( plane2_normal, Ld ) * plane1_dist ) / Ld.LengthSq();

	// Find intersection of line and first sphere
	float a = Ld.LengthSq();
	float b = 2 * ( Vec3::Dot( Lp, Ld ) - Vec3::Dot( Ld, P[0] ) );
	float c = Lp.LengthSq() + P[0].LengthSq() - 2 * Vec3::Dot( P[0], Lp ) - D[0]*D[0];

	float discriminant = b*b - 4*a*c;;
	if ( discriminant == 0 )
	{
		// Spheres only intersect in 1 point, so there is only 1 solution
		float t = ( -b + sqrtf( discriminant ) ) / ( 2 * a );
		return TrilaterationResult( { Lp + Ld * t } );
	}
	else if ( discriminant > 0 )
	{
		// Spheres intersect in 2 points, 2 possible solutions
		float t1 = ( -b + sqrtf( discriminant ) ) / ( 2 * a );
		float t2 = ( -b - sqrtf( discriminant ) ) / ( 2 * a );

		return TrilaterationResult( {
			Lp + Ld * t1,
			Lp + Ld * t2
		} );
	}
	else
	{
		// Spheres don't intersect at all
		// An estimate solution can still be formed by ignoring the complex part of the equation
		float t = ( -b ) / ( 2 * a );
		return TrilaterationResult( { Lp + Ld * t } );
	}
}