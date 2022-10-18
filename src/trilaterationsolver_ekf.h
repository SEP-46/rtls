#pragma once

#include "vector.h"
#include "matrix.h"
#include "trilaterationsolver.h"
#include "trilaterationsolver_basic.h"
#include <memory>

// Given the positions of at least 3 anchors, calculates the possible positions of the tag
// The output of this solver is filtered using the Extended Kalman Filter algorithm
class TrilaterationSolver_EKF : public ITrilaterationSolver
{
public:
	TrilaterationSolver_EKF();

	const char* GetName() const override { return "ekf"; }

	TrilaterationResult FindTagPosition( Timestamp_t timestamp, const Vec3* anchorPositions, const float* anchorDistances, size_t numAnchors ) override;

	void SetBounds( const AABB& bounds ) override;

private:
	static constexpr size_t MAX_ANCHORS = 4;
	using AnchorMat = DynMat<6, 6>;
	struct State
	{
		State() = default;
		State( const Mat6x1& mat )
		{
			position.x = mat[0][0]; position.y = mat[1][0]; position.z = mat[2][0];
			velocity.x = mat[3][0]; velocity.y = mat[4][0]; velocity.z = mat[5][0];
		}
		Mat6x1 AsMatrix() const { return { position.x, position.y, position.z, velocity.x, velocity.y, velocity.z }; }

		Vec3 position;
		Vec3 velocity;
	};

	State f( const State& X_last, float dt );
	AnchorMat h( const State& X_k, const Vec3* A, size_t N );

	Mat6x6 F( float dt );
	AnchorMat H( const State& X_est, const Vec3* A, size_t N );
	Mat6x6 Q( float dt );
	AnchorMat R( size_t N );

private:
	State mLastState;
	Mat6x6 mLastCovariance = Mat6x6::Zero();
	Timestamp_t mLastTimestamp = 0;
	bool mDidInitialGuess = false;
	TrilaterationSolver_Basic mBasicSolver;

	struct EKFSettings
	{
		float std_dev_accel = 0.5f;
		float std_dev_measurement_noise = 0.1f;
		float std_dev_position = 0.0f;
		float std_dev_velocity = 0.0f;
	};
	EKFSettings mSettings;
};