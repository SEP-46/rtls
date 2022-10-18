#include "trilaterationsolver_ekf.h"

TrilaterationSolver_EKF::TrilaterationSolver_EKF()
{
	mLastState.position = { 0.0f, 0.0f, 0.0f };
	mLastState.velocity = { 0.0f, 0.0f, 0.0f };
}

TrilaterationResult TrilaterationSolver_EKF::FindTagPosition(
	Timestamp_t timestamp,
	const Vec3* anchorPositions,
	const float* anchorDistances,
	size_t numAnchors )
{
	if ( numAnchors < 3 )
		return { mLastState.position, mLastState.velocity };

	if ( !mDidInitialGuess )
	{
		// Initially, try to use the basic algorithm to find the location of the anchors
		// Otherwise we will have to start at 0,0,0 and gradually find the correct position

		TrilaterationResult result = mBasicSolver.FindTagPosition( timestamp, anchorPositions, anchorDistances, numAnchors );
		mDidInitialGuess = true;

		if ( result.valid )
		{
			mLastState.position = result.position;
			mLastState.velocity = result.velocity;
			mLastTimestamp = timestamp;

			return result;
		}
	}

	float dt = (timestamp - mLastTimestamp) / 1000.0f;

	// Aliases
	const State& X_last = mLastState;
	const Mat6x6& P_last = mLastCovariance;
	const Vec3* A = anchorPositions;
	size_t N = numAnchors;
	const Mat6x6 I = Mat6x6::Identity();

	AnchorMat z_k( N, 1 );
	for ( size_t i = 0; i < N; i++ )
		z_k[i][0] = anchorDistances[i];

	// Matrices initialisation
	Mat6x6 F_k = F( dt );
	Mat6x6 Q_k = Q( dt );

	// Predict step
	State X_est = f( X_last, dt );
	Mat6x6 P_est = F_k * P_last * F_k.Transposed() + Q_k;

	AnchorMat H_k = H( X_est, A, N );
	AnchorMat H_k_T = H_k.Transposed();
	AnchorMat R_k = R( N );
	AnchorMat PH_T = P_est * H_k_T;

	// Update step
	AnchorMat y_k = z_k - h( X_est, A, N );
	AnchorMat S_k = H_k * PH_T + R_k;
	AnchorMat K_k = PH_T * S_k.Inverse();
	State X_k = X_est.AsMatrix() + K_k * y_k;
	Mat6x6 P_k = (I - K_k * H_k) * P_est;

	mLastState = X_k;
	mLastCovariance = P_k;
	mLastTimestamp = timestamp;

	return { X_k.position, X_k.velocity };
}

void TrilaterationSolver_EKF::SetBounds( const AABB& bounds )
{
	mBasicSolver.SetBounds( bounds );
}

Mat6x6 TrilaterationSolver_EKF::F( float dt )
{
	return {
		1.0f, 0.0f, 0.0f, dt,   0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, dt,   0.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f, dt,
		0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};
}

TrilaterationSolver_EKF::AnchorMat TrilaterationSolver_EKF::H( const State& X_est, const Vec3* A, size_t N )
{
	AnchorMat out( N, 6 );
	for ( size_t i = 0; i < N; i++ )
	{
		float r = Vec3::Distance( X_est.position, A[i] );

		if ( r != 0.0f )
		{
			out[i][0] = (X_est.position.x - A[i].x) / r;
			out[i][1] = (X_est.position.y - A[i].y) / r;
			out[i][2] = (X_est.position.z - A[i].z) / r;
		}
		else
		{
			out[i][0] = 9999999;
			out[i][1] = 9999999;
			out[i][2] = 9999999;
		}
		out[i][3] = 0.0f;
		out[i][4] = 0.0f;
		out[i][5] = 0.0f;
	}
	return out;
}

TrilaterationSolver_EKF::State TrilaterationSolver_EKF::f( const State& X_last, float dt )
{
	State X_k;
	X_k.position = X_last.position + X_last.velocity * dt;
	X_k.velocity = X_last.velocity;
	return X_k;
}

TrilaterationSolver_EKF::AnchorMat TrilaterationSolver_EKF::h( const State& X_est, const Vec3* A, size_t N )
{
	AnchorMat out( N, 1 );
	for ( size_t i = 0; i < N; i++ )
	{
		out[i][0] = Vec3::Distance(X_est.position, A[i] );
	}
	return out;
}

Mat6x6 TrilaterationSolver_EKF::Q( float dt )
{
	const float var_accel = mSettings.std_dev_accel * mSettings.std_dev_accel;
	const float dt2 = dt * dt * var_accel;
	const float half_dt3 = 0.5f * dt2 * dt;
	const float quar_dt4 = 0.5f * half_dt3 * dt;

	return {
		quar_dt4, 0.0f,     0.0f,     half_dt3, 0.0f,     0.0f,
		0.0f,     quar_dt4, 0.0f,     0.0f,     half_dt3, 0.0f,
		0.0f,     0.0f,     quar_dt4, 0.0f,     0.0f,     half_dt3,
		half_dt3, 0.0f,     0.0f,     dt2,      0.0f,     0.0f,
		0.0f,     half_dt3, 0.0f,     0.0f,     dt2,      0.0f,
		0.0f,     0.0f,     half_dt3, 0.0f,     0.0f,     dt2
	};
}

TrilaterationSolver_EKF::AnchorMat TrilaterationSolver_EKF::R( size_t N )
{
	return AnchorMat::Diagonal( N, mSettings.std_dev_measurement_noise * mSettings.std_dev_measurement_noise );
}
