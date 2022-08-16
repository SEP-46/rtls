#include <gtest/gtest.h>

#include "vector.h"
#include <cmath>

TEST( Vec3Test, Assign )
{
	Vec3 a = { 1.0f, 2.0f, 3.0f };
	Vec3 b = a;
	Vec3 c;
	c.x = 1.0f;
	c.y = 2.0f;
	c.z = 3.0f;

	EXPECT_EQ( a, b );
	EXPECT_EQ( b, c );
}

TEST( Vec3Test, Add )
{
	Vec3 a = { 1.0f, 2.0f, 3.0f };
	const Vec3 b = { 4.0f, 5.0f, 6.0f };
	EXPECT_EQ( a + b, Vec3( 5.0f, 7.0f, 9.0f ) );

	a += b;
	EXPECT_EQ( a, Vec3( 5.0f, 7.0f, 9.0f ) );
}

TEST( Vec3Test, Subtract )
{
	Vec3 a = { 1.0f, 10.0f, 100.0f };
	const Vec3 b = { 4.0f, 5.0f, 6.0f };
	EXPECT_EQ( a - b, Vec3( -3.0f, 5.0f, 94.0f ) );

	a -= b;
	EXPECT_EQ( a, Vec3( -3.0f, 5.0f, 94.0f ) );
}

TEST( Vec3Test, MultiplyScalar )
{
	Vec3 a = { 2.0f, 4.0f, 6.0f };
	EXPECT_EQ( a * 0.5f, Vec3( 1.0f, 2.0f, 3.0f ) );

	a *= 0.5f;
	EXPECT_EQ( a, Vec3( 1.0f, 2.0f, 3.0f ) );
}

TEST( Vec3Test, DivideScalar )
{
	Vec3 a = { 2.0f, 4.0f, 6.0f };
	EXPECT_EQ( a / 2.0f, Vec3( 1.0f, 2.0f, 3.0f ) );

	a /= 2.0f;
	EXPECT_EQ( a, Vec3( 1.0f, 2.0f, 3.0f ) );
}

TEST( Vec3Test, Negate )
{
	Vec3 a = { 1.0f, 2.0f, 3.0f };
	EXPECT_EQ( -a, Vec3( -1.0f, -2.0f, -3.0f ) );
}

TEST( Vec3Test, Equals )
{
	Vec3 a = { 1.0f, 2.0f, 3.0f };
	Vec3 b = { 1.0f, 2.0f, 3.0f };
	EXPECT_TRUE( a == b );
}

TEST( Vec3Test, NotEquals )
{
	Vec3 a = { 1.0f, 2.0f, 3.0f };
	Vec3 b = { 1.0f, 2.0f, 4.0f };
	EXPECT_TRUE( a != b );
}

TEST( Vec3Test, Length )
{
	Vec3 a = { 1.0f, 2.0f, 3.0f };
	EXPECT_FLOAT_EQ( a.LengthSq(), 14.0f );
	EXPECT_FLOAT_EQ( a.Length(), sqrtf( 14.0f ) );
}

TEST( Vec3Test, Normalize )
{
	const Vec3 a = { 0.0f, 0.0f, 1.0f };
	EXPECT_EQ( a.Normalized(), Vec3( 0.0f, 0.0f, 1.0f ) );

	const Vec3 b = { 0.0f, 3.0f, 0.0f };
	EXPECT_EQ( b.Normalized(), Vec3( 0.0f, 1.0f, 0.0f ) );
}

TEST( Vec3Test, NormalizeMatchesNormalized )
{
	const Vec3 a = { 1.0f, 2.0f, 3.0f };
	Vec3 b = { 1.0f, 2.0f, 3.0f };
	float length = b.Normalize();
	EXPECT_EQ( a.Normalized(), b );
	EXPECT_FLOAT_EQ( a.Length(), length );
}

TEST( Vec3Test, Dot )
{
	const Vec3 a = { 1.0f, 2.0f, 3.0f };
	const Vec3 b = { 4.0f, 5.0f, 6.0f };
	EXPECT_FLOAT_EQ( Vec3::Dot( a, b ), 32.0f );
	EXPECT_FLOAT_EQ( Vec3::Dot( a, a ), a.LengthSq() );
}

TEST( Vec3Test, Cross )
{
	const Vec3 a = { 1.0f, 0.0f, 0.0f };
	const Vec3 b = { 0.0f, 1.0f, 0.0f };
	EXPECT_EQ( Vec3::Cross( a, b ), Vec3( 0.0f, 0.0f, 1.0f ) );
}

TEST( Vec3Test, Lerp )
{
	const Vec3 a = { 0.0f, 0.0f, 0.0f };
	const Vec3 b = { 10.0f, 10.0f, 10.0f };
	EXPECT_EQ( Vec3::Lerp( a, b, 0.0f ), a );
	EXPECT_EQ( Vec3::Lerp( a, b, 0.5f ), Vec3( 5.0f, 5.0f, 5.0f ) );
	EXPECT_EQ( Vec3::Lerp( a, b, 1.0f ), b );
}

TEST( Vec3Test, Floor )
{
	const Vec3 a = { 1.99f, 1.01f, 2.5f };
	EXPECT_EQ( Vec3::Floor( a ), Vec3( 1.0f, 1.0f, 2.0f ) );
}

TEST( Vec3Test, Reciprocal )
{
	const Vec3 a = { 1.0f, 1.0f, 1.0f };
	EXPECT_EQ( Vec3::Reciprocal( a ), a );
	EXPECT_EQ( Vec3::Reciprocal( Vec3( 10.0f, 5.0f, 2.0f ) ), Vec3( 0.1f, 0.2f, 0.5f ) );
}

TEST( Vec3Test, CloseEnough )
{
	const float epsilon = 0.001f;

	const Vec3 a = { 1.0f, 1.0f, 1.0f };
	Vec3 b = { 1.0f, 1.0f, 1.0f };
	b.x += epsilon * 0.5f;
	b.y -= epsilon * 0.5f;
	b.z -= epsilon * 0.9f;
	EXPECT_TRUE( Vec3::CloseEnough( a, b, epsilon ) );
}