#pragma once

#include "mathlib.h"

template <typename T>
class TVec3
{
public:
	TVec3() = default;
	TVec3( T x, T y, T z ) :
		x( x ), y( y ), z( z )
	{}

	template <typename U>
	explicit operator TVec3<U>() const
	{
		return TVec3<U>( (U)x, (U)y, (U)z );
	}

	bool operator==( const TVec3& rhs ) const
	{
		return x == rhs.x && y == rhs.y && z == rhs.z;
	}
	bool operator!=( const TVec3& rhs ) const
	{
		return x != rhs.x || y != rhs.y || z != rhs.z;
	}
	TVec3 operator*( const TVec3& rhs ) const
	{
		TVec3 res;
		res.x = x * rhs.x;
		res.y = y * rhs.y;
		res.z = z * rhs.z;

		return res;
	}
	TVec3 operator*( T scalar ) const
	{
		TVec3 res;
		res.x = x * scalar;
		res.y = y * scalar;
		res.z = z * scalar;

		return res;
	}
	TVec3 operator/( T scalar ) const
	{
		TVec3 res;
		res.x = x / scalar;
		res.y = y / scalar;
		res.z = z / scalar;

		return res;
	}
	TVec3 operator-() const
	{
		TVec3 res;
		res.x = -x;
		res.y = -y;
		res.z = -z;

		return res;
	}
	TVec3 operator+( const TVec3& rhs ) const
	{
		TVec3 res;
		res.x = x + rhs.x;
		res.y = y + rhs.y;
		res.z = z + rhs.z;

		return res;
	}
	TVec3 operator-( const TVec3& rhs ) const
	{
		TVec3 res;
		res.x = x - rhs.x;
		res.y = y - rhs.y;
		res.z = z - rhs.z;

		return res;
	}
	TVec3& operator+=( const TVec3& rhs )
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;

		return *this;
	}
	TVec3& operator-=( const TVec3& rhs )
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;

		return *this;
	}
	TVec3& operator*=( T scalar )
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;

		return *this;
	}
	TVec3& operator/=( T scalar )
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;

		return *this;
	}

	T LengthSq() const
	{
		return x * x + y * y + z * z;
	}

	T Length() const
	{
		return sqrtf( LengthSq() );
	}

	T Normalize()
	{
		T length = Length();
		x /= length;
		y /= length;
		z /= length;

		return length;
	}

	TVec3 Normalized() const
	{
		T length = Length();

		return { (T)(x / length), (T)(y / length), (T)(z / length) };
	}

	static T DistanceSq( const TVec3& start, const TVec3& end )
	{
		return ( end - start ).LengthSq();
	}

	static T Distance( const TVec3& start, const TVec3& end )
	{
		return ( end - start ).Length();
	}

	static T Dot( const TVec3& a, const TVec3& b )
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	static TVec3 Cross( const TVec3& a, const TVec3& b )
	{
		// |  x    y    z  |
		// | a.x  a.y  a.z |
		// | b.x  b.y  b.z |
		return {
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
		};
	}

	static TVec3 Lerp( const TVec3& a, const TVec3& b, float t )
	{
		return TVec3( ::Lerp( a.x, b.x, t ),
			::Lerp( a.y, b.y, t ),
			::Lerp( a.z, b.z, t ) );
	}

	static TVec3 Floor( const TVec3& v )
	{
		return {
			floorf( v.x ),
			floorf( v.y ),
			floorf( v.z )
		};
	}

	static TVec3 Reciprocal( const TVec3& v )
	{
		return { 1.0f / v.x, 1.0f / v.y, 1.0f / v.z };
	}

	static bool CloseEnough( const TVec3& a, const TVec3& b, float epsilon = 0.001f )
	{
		return ::CloseEnough( a.x, b.x, epsilon ) &&
			::CloseEnough( a.y, b.y, epsilon ) &&
			::CloseEnough( a.z, b.z, epsilon );
	}
public:
	T x;
	T y;
	T z;
};

using Vec3 = TVec3<float>;
using Int3 = TVec3<int>;
using UInt3 = TVec3<unsigned int>;