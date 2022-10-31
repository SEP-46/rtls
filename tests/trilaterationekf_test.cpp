#include <gtest/gtest.h>

#include "trilaterationsolver_ekf.h"
#include <thread>
#include <chrono>
#include <random>
using namespace std::chrono_literals;

static void CalculateDistances( const Vec3& tag, const Vec3 anchors[3], float* out )
{
	for ( int i = 0; i < 3; i++ )
	{
		out[i] = Vec3::Distance( tag, anchors[i] );
	}
}


static constexpr float MEAN = 0.0f;
static constexpr float STDDEV = 0.01f;
static std::default_random_engine gRNG;
static std::normal_distribution<float> gNoise( MEAN, STDDEV );

static void CalculateDistancesWithNoise( const Vec3& tag, const Vec3 anchors[3], float* out )
{
	for ( int i = 0; i < 3; i++ )
	{
		out[i] = Vec3::Distance( tag, anchors[i] ) + gNoise( gRNG );
	}
}

TEST( TrilaterationEKF, BasicCase )
{
	Vec3 anchors[] = {
		Vec3( 0.0f, 0.0f, 0.0f ),
		Vec3( 0.5f, 0.0f, 0.0f ),
		Vec3( 0.0f, 0.5f, 0.0f )
	};

	float distances[3];
	CalculateDistances( { 0.5f, 0.5f, 0.0f }, anchors, distances );

	TrilaterationSolver_EKF solver;
	TrilaterationResult result = solver.FindTagPosition( 0, anchors, distances, 3 );
	EXPECT_TRUE( result.valid );
	EXPECT_TRUE( Vec3::CloseEnough( result.position, Vec3( 0.5f, 0.5f, 0.0f ) ) );
}

TEST( TrilaterationEKF, BasicCaseWithNoise )
{
	Vec3 anchors[] = {
		Vec3( 0.0f, 0.0f, 0.0f ),
		Vec3( 0.5f, 0.0f, 0.0f ),
		Vec3( 0.0f, 0.5f, 0.0f )
	};

	TrilaterationSolver_EKF solver;
	float distances[3];
	TrilaterationResult result;

	Timestamp_t time = 0;
	for ( int i = 0; i < 10000; i++ )
	{
		CalculateDistancesWithNoise( { 0.5f, 0.5f, 0.0f }, anchors, distances );
		result = solver.FindTagPosition( time, anchors, distances, 3 );
		time += 100;
	}

	EXPECT_TRUE( result.valid );
	EXPECT_TRUE( Vec3::CloseEnough( result.position, Vec3( 0.5f, 0.5f, 0.0f ), 0.1f ) );
}

TEST( TrilaterationEKF, MovingTagCase )
{
	Vec3 anchors[] = {
		Vec3( 0.0f, 0.0f, 0.0f ),
		Vec3( 0.5f, 0.0f, 0.0f ),
		Vec3( 0.0f, 0.5f, 0.0f )
	};

	TrilaterationSolver_EKF solver;
	float distances[3];
	TrilaterationResult result;
	Timestamp_t time = 0;

	// Start tag at (0.5, 0.5, 0)
	Vec3 tag = { 0.5f, 0.5f, 0.0f };
	CalculateDistances( { 0.5f, 0.5f, 0.0f }, anchors, distances );
	result = solver.FindTagPosition( time, anchors, distances, 3 );

	// Move tag to (1, 0.5, 0) in 0.5s
	for ( int i = 0; i < 5; i++ )
	{
		tag.x += 0.1f;
		time += 100;
		CalculateDistances( { 1.0f, 0.5f, 0.0f }, anchors, distances );
		result = solver.FindTagPosition( time, anchors, distances, 3 );
	}

	// Make tag stationary for 5s
	for ( int i = 0; i < 50; i++ )
	{
		time += 100;
		result = solver.FindTagPosition( time, anchors, distances, 3 );
	}
	
	EXPECT_TRUE( result.valid );
	EXPECT_TRUE( Vec3::CloseEnough( result.position, Vec3( 1.0f, 0.5f, 0.0f ), 0.01f ) ); // Using epsilon of 0.01, accurate within 1cm
}

TEST( TrilaterationEKF, MovingTagCaseWithNoise )
{
	Vec3 anchors[] = {
		Vec3( 0.0f, 0.0f, 0.0f ),
		Vec3( 0.5f, 0.0f, 0.0f ),
		Vec3( 0.0f, 0.5f, 0.0f )
	};

	TrilaterationSolver_EKF solver;
	float distances[3];
	TrilaterationResult result;
	Timestamp_t time = 0;

	// Start tag at (0.5, 0.5, 0)
	Vec3 tag = { 0.5f, 0.5f, 0.0f };
	CalculateDistancesWithNoise( { 0.5f, 0.5f, 0.0f }, anchors, distances );
	result = solver.FindTagPosition( time, anchors, distances, 3 );

	// Move tag to (1, 0.5, 0) in 0.5s
	for ( int i = 0; i < 5; i++ )
	{
		tag.x += 0.1f;
		time += 100;
		CalculateDistancesWithNoise( { 1.0f, 0.5f, 0.0f }, anchors, distances );
		result = solver.FindTagPosition( time, anchors, distances, 3 );
	}

	// Make tag stationary for 0.5s
	for ( int i = 0; i < 5; i++ )
	{
		time += 100;
		result = solver.FindTagPosition( time, anchors, distances, 3 );
	}

	EXPECT_TRUE( result.valid );
	EXPECT_TRUE( Vec3::CloseEnough( result.position, Vec3( 1.0f, 0.5f, 0.0f ), 0.05f ) ); // Using epsilon of 0.05, accurate within 5cm
}

TEST( TrilaterationEKF, NonIntersecting )
{
	Vec3 anchors[] = {
		Vec3( 0.0f, 0.0f, 0.0f ),
		Vec3( 0.5f, 0.0f, 0.0f ),
		Vec3( 0.0f, 0.5f, 0.0f )
	};

	float distances[] = {
		0.174f,
		0.23f,
		0.188f
	};

	TrilaterationSolver_EKF solver;
	TrilaterationResult result;
	Timestamp_t time = 0;

	for ( int i = 0; i < 100; i++ )
	{
		result = solver.FindTagPosition( time, anchors, distances, 3 );
		time += 100;
	}
	EXPECT_TRUE( result.valid );
}