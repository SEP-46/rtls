#include <gtest/gtest.h>

#include "matrix.h"
#include <cmath>

TEST( MatTest, Zero )
{
	Mat6x6 a = Mat6x6::Zero();
	for ( int i = 0; i < 6; i++ )
	{
		for ( int j = 0; j < 6; j++ )
		{
			EXPECT_FLOAT_EQ( a[i][j], 0 );
		}
	}
}

TEST( MatTest, Assign )
{
	Mat6x6 a = Mat6x6::Zero();
	a[0][0] = 1.0f;
	Mat6x6 b = a;
	EXPECT_FLOAT_EQ( b[0][0], 1.0f );
}

TEST( MatTest, Equals )
{
	Mat6x6 a = Mat6x6::Identity();
	Mat6x6 b = Mat6x6::Identity();
	EXPECT_EQ( a, b );
}

TEST( MatTest, Identity )
{
	Mat6x6 I = Mat6x6::Identity();
	Mat<6, 1> A = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f };
	Mat<6, 1> X = I * A;
	EXPECT_EQ( A, X );
}

TEST( MatTest, Mult )
{
	Mat<2, 2> A = {
		1.0f, 2.0f,
		3.0f, 4.0f
	};
	Mat<2, 1> B = {
		5.0f,
		6.0f
	};
	Mat<2, 1> C = A * B;
	EXPECT_FLOAT_EQ( C[0][0], 17.0f );
	EXPECT_FLOAT_EQ( C[1][0], 39.0f );
}

TEST( MatTest, MultScalar )
{
	Mat<2, 2> A = Mat<2, 2>::Identity();
	Mat<2, 2> C = A * 2;
	EXPECT_FLOAT_EQ( C[0][0], 2.0f );
	EXPECT_FLOAT_EQ( C[0][1], 0.0f );
	EXPECT_FLOAT_EQ( C[1][0], 0.0f );
	EXPECT_FLOAT_EQ( C[1][1], 2.0f );
}

TEST( MatTest, Add )
{
	Mat<2, 2> A = Mat<2, 2>::Identity() + Mat<2, 2>::Identity();
	EXPECT_FLOAT_EQ( A[0][0], 2.0f );
	EXPECT_FLOAT_EQ( A[0][1], 0.0f );
	EXPECT_FLOAT_EQ( A[1][0], 0.0f );
	EXPECT_FLOAT_EQ( A[1][1], 2.0f );
}

TEST( MatTest, Subtract )
{
	Mat<2, 2> A = Mat<2, 2>::Identity() - Mat<2, 2>::Identity();
	EXPECT_FLOAT_EQ( A[0][0], 0.0f );
	EXPECT_FLOAT_EQ( A[0][1], 0.0f );
	EXPECT_FLOAT_EQ( A[1][0], 0.0f );
	EXPECT_FLOAT_EQ( A[1][1], 0.0f );
}

TEST( MatTest, Transpose )
{
	Mat<2, 2> A = {
		1.0f, 2.0f,
		3.0f, 4.0f
	};
	Mat<2, 2> C = A.Transposed();
	EXPECT_FLOAT_EQ( C[0][0], 1.0f );
	EXPECT_FLOAT_EQ( C[0][1], 3.0f );
	EXPECT_FLOAT_EQ( C[1][0], 2.0f );
	EXPECT_FLOAT_EQ( C[1][1], 4.0f );
}