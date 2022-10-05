#pragma once

#include <cassert>
#include <cmath>
#include <cstring>
#include <initializer_list>

// Fixed size matrix class
template <size_t Rows, size_t Columns>
struct Mat
{
	using T = float;
	
	Mat() = default;

	Mat( std::initializer_list<T> l )
	{
		assert( l.size() == Rows * Columns );
		memcpy( m, l.begin(), l.size() * sizeof( T ) );
	}

	static Mat Zero()
	{
		Mat mat;
		memset( mat.m, 0, sizeof( mat.m ) );
		return mat;
	}

	static Mat Diagonal( T val )
	{
		static_assert(Rows == Columns, "Cannot have diagonal for non-square matrix");

		Mat mat = Mat::Zero();
		for ( size_t i = 0; i < Rows; i++ )
		{
			mat[i][i] = val;
		}
		return mat;
	}

	static Mat Identity()
	{
		return Mat::Diagonal( 1 );
	}

	// Careful! Multiplication in C++ is left associative, so something like A * B * C will evaluate as (A * B) * C
	// This could give different results than you might be expecting, based on maths notation, which goes right to left
	template <size_t OtherRows, size_t OtherColumns>
	Mat<Rows, OtherColumns> operator*( const Mat<OtherRows, OtherColumns>& rhs ) const
	{
		static_assert( Columns == OtherRows, "Invalid matrix dimensions for multiplication" );

		Mat<Rows, OtherColumns> result = Mat<Rows, OtherColumns>::Zero();
		for ( size_t y = 0; y < Rows; y++ )
		{
			for ( size_t x = 0; x < OtherColumns; x++ )
			{
				for ( size_t i = 0; i < Columns; i++ )
				{
					result[y][x] += m[y][i] * rhs[i][x];
				}
			}
		}
		return result;
	}

	Mat operator*( T val )
	{
		Mat result;
		for ( size_t y = 0; y < Rows; y++ )
		{
			for ( size_t x = 0; x < Columns; x++ )
			{
				result[y][x] = m[y][x] * val;
			}
		}
		return result;
	}

	Mat operator+( const Mat& rhs ) const
	{
		Mat result;
		for ( size_t y = 0; y < Rows; y++ )
		{
			for ( size_t x = 0; x < Columns; x++ )
			{
				result[y][x] = m[y][x] + rhs[y][x];
			}
		}
		return result;
	}

	Mat operator-( const Mat& rhs ) const
	{
		Mat result;
		for ( size_t y = 0; y < Rows; y++ )
		{
			for ( size_t x = 0; x < Columns; x++ )
			{
				result[y][x] = m[y][x] - rhs[y][x];
			}
		}
		return result;
	}

	Mat<Columns, Rows> Transposed() const
	{
		Mat<Columns, Rows> result;
		for ( size_t y = 0; y < Rows; y++ )
		{
			for ( size_t x = 0; x < Columns; x++ )
			{
				result[x][y] = m[y][x];
			}
		}
		return result;
	}

	bool operator==( const Mat& rhs ) const
	{
		for ( size_t y = 0; y < Rows; y++ )
		{
			for ( size_t x = 0; x < Columns; x++ )
			{
				if ( m[y][x] != rhs[y][x] )
					return false;
			}
		}
		return true;
	}

	const T* operator[]( size_t index ) const { return m[index]; }
	T* operator[]( size_t index ) { return m[index]; }

public:
	T m[Rows][Columns];
};

// Dynamic size matrix class (must still specify maximum possible size)
template <size_t MaxRows, size_t MaxColumns>
struct DynMat
{
	using T = float;

	DynMat( size_t rows, size_t columns ) :
		rows( rows ),
		columns( columns )
	{
		assert( rows <= MaxRows );
		assert( columns <= MaxColumns );
	}

	static DynMat Zero( size_t rows, size_t columns )
	{
		DynMat mat( rows, columns );
		memset( mat.m, 0, sizeof( m ) );
		return mat;
	}

	static DynMat Diagonal( size_t rows, T val )
	{
		DynMat mat = DynMat::Zero( rows, rows );
		for ( size_t i = 0; i < rows; i++ )
		{
			mat[i][i] = val;
		}
		return mat;
	}

	// Careful! Multiplication in C++ is left associative, so something like A * B * C will evaluate as (A * B) * C
	// This could give different results than you might be expecting, based on maths notation, which goes right to left
	template <size_t OtherRows, size_t OtherColumns>
	DynMat<MaxRows, OtherColumns> operator*( const Mat<OtherRows, OtherColumns>& rhs ) const
	{
		assert( columns == OtherRows && "Invalid matrix dimensions for multiplication" );

		DynMat<MaxRows, OtherColumns> result( rows, OtherColumns );
		for ( size_t y = 0; y < rows; y++ )
		{
			for ( size_t x = 0; x < OtherColumns; x++ )
			{
				result[y][x] = 0.0f;
				for ( size_t i = 0; i < columns; i++ )
				{
					result[y][x] += m[y][i] * rhs[i][x];
				}
			}
		}
		return result;
	}
	// Careful! Multiplication in C++ is left associative, so something like A * B * C will evaluate as (A * B) * C
	// This could give different results than you might be expecting, based on maths notation, which goes right to left
	template <size_t OtherRows, size_t OtherColumns>
	DynMat<MaxRows, OtherColumns> operator*( const DynMat<OtherRows, OtherColumns>& rhs ) const
	{
		assert( columns == rhs.rows && "Invalid matrix dimensions for multiplication" );

		DynMat<MaxRows, OtherColumns> result( rows, rhs.columns );
		for ( size_t y = 0; y < rows; y++ )
		{
			for ( size_t x = 0; x < rhs.columns; x++ )
			{
				result[y][x] = 0.0f;
				for ( size_t i = 0; i < columns; i++ )
				{
					result[y][x] += m[y][i] * rhs[i][x];
				}
			}
		}
		return result;
	}

	DynMat operator*( T val )
	{
		DynMat result( rows, columns );
		for ( size_t y = 0; y < rows; y++ )
		{
			for ( size_t x = 0; x < columns; x++ )
			{
				result[y][x] = m[y][x] * val;
			}
		}
		return result;
	}

	DynMat operator+( const DynMat& rhs ) const
	{
		assert( rows == rhs.rows && columns == rhs.columns );

		DynMat result( rows, columns );
		for ( size_t y = 0; y < rows; y++ )
		{
			for ( size_t x = 0; x < columns; x++ )
			{
				result[y][x] = m[y][x] + rhs[y][x];
			}
		}
		return result;
	}

	DynMat operator-( const DynMat& rhs ) const
	{
		assert( rows == rhs.rows && columns == rhs.columns );

		DynMat result( rows, columns );
		for ( size_t y = 0; y < rows; y++ )
		{
			for ( size_t x = 0; x < columns; x++ )
			{
				result[y][x] = m[y][x] - rhs[y][x];
			}
		}
		return result;
	}

	DynMat<MaxColumns, MaxRows> Transposed() const
	{
		DynMat<MaxColumns, MaxRows> result( columns, rows );
		for ( size_t y = 0; y < rows; y++ )
		{
			for ( size_t x = 0; x < columns; x++ )
			{
				result[x][y] = m[y][x];
			}
		}
		return result;
	}

	DynMat Inverse() const
	{
		// Calculating the inverse of a matrix quickly is hard, so we only handle 2 specialised cases here
		// The 2 cases are 3x3 matrix and 4x4 matrix, which are used when we have 3 anchors and 4 anchors respectively
		// If we ever need to support more than 4 anchors at a time, then this method should be updated
		assert( rows == columns && "Inverse for non-square matrix unimplemented" );
		assert( ( rows == 3 || rows == 4 ) && "Inverse for non 3x3/4x4 matrix unimplemented" );

		if ( rows == 3 )
		{
			// From https://stackoverflow.com/questions/983999/simple-3x3-matrix-inverse-code-c

			float det = m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2]) -
				m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
				m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);

			float invdet = 1 / det;

			DynMat inv( rows, columns );
			inv[0][0] = (m[1][1] * m[2][2] - m[2][1] * m[1][2]) * invdet;
			inv[0][1] = (m[0][2] * m[2][1] - m[0][1] * m[2][2]) * invdet;
			inv[0][2] = (m[0][1] * m[1][2] - m[0][2] * m[1][1]) * invdet;
			inv[1][0] = (m[1][2] * m[2][0] - m[1][0] * m[2][2]) * invdet;
			inv[1][1] = (m[0][0] * m[2][2] - m[0][2] * m[2][0]) * invdet;
			inv[1][2] = (m[1][0] * m[0][2] - m[0][0] * m[1][2]) * invdet;
			inv[2][0] = (m[1][0] * m[2][1] - m[2][0] * m[1][1]) * invdet;
			inv[2][1] = (m[2][0] * m[0][1] - m[0][0] * m[2][1]) * invdet;
			inv[2][2] = (m[0][0] * m[1][1] - m[1][0] * m[0][1]) * invdet;
			return inv;
		}
		else if ( rows == 4 )
		{
			DynMat inv( rows, columns );
			float wtmp[4][8];
			float m0, m1, m2, m3, s;
			float* r0;
			float* r1;
			float* r2;
			float* r3;

			r0 = wtmp[0]; r1 = wtmp[1]; r2 = wtmp[2]; r3 = wtmp[3];

			r0[0] = m[0][0]; r0[1] = m[0][1];
			r0[2] = m[0][2]; r0[3] = m[0][3];
			r0[4] = 1.0f; r0[5] = r0[6] = r0[7] = 0.0f;

			r1[0] = m[1][0]; r1[1] = m[1][1];
			r1[2] = m[1][2]; r1[3] = m[1][3];
			r1[5] = 1.0f; r1[4] = r1[6] = r1[7] = 0.0f;

			r2[0] = m[2][0]; r2[1] = m[2][1];
			r2[2] = m[2][2]; r2[3] = m[2][3];
			r2[6] = 1.0f; r2[4] = r2[5] = r2[7] = 0.0f;

			r3[0] = m[3][0]; r3[1] = m[3][1];
			r3[2] = m[3][2]; r3[3] = m[3][3];
			r3[7] = 1.0f; r3[4] = r3[5] = r3[6] = 0.0f;

			/* choose pivot - or die */
			if ( fabsf( r3[0] ) > fabsf( r2[0] ) ) std::swap( r3, r2 );
			if ( fabsf( r2[0] ) > fabsf( r1[0] ) ) std::swap( r2, r1 );
			if ( fabsf( r1[0] ) > fabsf( r0[0] ) ) std::swap( r1, r0 );
			if ( 0.0f == r0[0] )  return inv;

			/* eliminate first variable     */
			m1 = r1[0] / r0[0]; m2 = r2[0] / r0[0]; m3 = r3[0] / r0[0];
			s = r0[1]; r1[1] -= m1 * s; r2[1] -= m2 * s; r3[1] -= m3 * s;
			s = r0[2]; r1[2] -= m1 * s; r2[2] -= m2 * s; r3[2] -= m3 * s;
			s = r0[3]; r1[3] -= m1 * s; r2[3] -= m2 * s; r3[3] -= m3 * s;
			s = r0[4];
			if ( s != 0.0f ) { r1[4] -= m1 * s; r2[4] -= m2 * s; r3[4] -= m3 * s; }
			s = r0[5];
			if ( s != 0.0f ) { r1[5] -= m1 * s; r2[5] -= m2 * s; r3[5] -= m3 * s; }
			s = r0[6];
			if ( s != 0.0f ) { r1[6] -= m1 * s; r2[6] -= m2 * s; r3[6] -= m3 * s; }
			s = r0[7];
			if ( s != 0.0f ) { r1[7] -= m1 * s; r2[7] -= m2 * s; r3[7] -= m3 * s; }

			/* choose pivot - or die */
			if ( fabsf( r3[1] ) > fabsf( r2[1] ) ) std::swap( r3, r2 );
			if ( fabsf( r2[1] ) > fabsf( r1[1] ) ) std::swap( r2, r1 );
			if ( 0.0f == r1[1] )  return inv;

			/* eliminate second variable */
			m2 = r2[1] / r1[1]; m3 = r3[1] / r1[1];
			r2[2] -= m2 * r1[2]; r3[2] -= m3 * r1[2];
			r2[3] -= m2 * r1[3]; r3[3] -= m3 * r1[3];
			s = r1[4]; if ( 0.0f != s ) { r2[4] -= m2 * s; r3[4] -= m3 * s; }
			s = r1[5]; if ( 0.0f != s ) { r2[5] -= m2 * s; r3[5] -= m3 * s; }
			s = r1[6]; if ( 0.0f != s ) { r2[6] -= m2 * s; r3[6] -= m3 * s; }
			s = r1[7]; if ( 0.0f != s ) { r2[7] -= m2 * s; r3[7] -= m3 * s; }

			/* choose pivot - or die */
			if ( fabsf( r3[2] ) > fabsf( r2[2] ) ) std::swap( r3, r2 );
			if ( 0.0f == r2[2] )  return inv;

			/* eliminate third variable */
			m3 = r3[2] / r2[2];
			r3[3] -= m3 * r2[3]; r3[4] -= m3 * r2[4];
			r3[5] -= m3 * r2[5]; r3[6] -= m3 * r2[6];
			r3[7] -= m3 * r2[7];

			/* last check */
			if ( 0.0f == r3[3] ) return inv;

			s = 1.0f / r3[3];             /* now back substitute row 3 */
			r3[4] *= s; r3[5] *= s; r3[6] *= s; r3[7] *= s;

			m2 = r2[3];                 /* now back substitute row 2 */
			s = 1.0f / r2[2];
			r2[4] = s * (r2[4] - r3[4] * m2); r2[5] = s * (r2[5] - r3[5] * m2);
			r2[6] = s * (r2[6] - r3[6] * m2); r2[7] = s * (r2[7] - r3[7] * m2);
			m1 = r1[3];
			r1[4] -= r3[4] * m1; r1[5] -= r3[5] * m1;
			r1[6] -= r3[6] * m1; r1[7] -= r3[7] * m1;
			m0 = r0[3];
			r0[4] -= r3[4] * m0; r0[5] -= r3[5] * m0;
			r0[6] -= r3[6] * m0; r0[7] -= r3[7] * m0;

			m1 = r1[2];                 /* now back substitute row 1 */
			s = 1.0f / r1[1];
			r1[4] = s * (r1[4] - r2[4] * m1); r1[5] = s * (r1[5] - r2[5] * m1);
			r1[6] = s * (r1[6] - r2[6] * m1); r1[7] = s * (r1[7] - r2[7] * m1);
			m0 = r0[2];
			r0[4] -= r2[4] * m0; r0[5] -= r2[5] * m0,
				r0[6] -= r2[6] * m0; r0[7] -= r2[7] * m0;

			m0 = r0[1];                 /* now back substitute row 0 */
			s = 1.0f / r0[0];
			r0[4] = s * (r0[4] - r1[4] * m0); r0[5] = s * (r0[5] - r1[5] * m0);
			r0[6] = s * (r0[6] - r1[6] * m0); r0[7] = s * (r0[7] - r1[7] * m0);

			inv.m[0][0] = r0[4]; inv.m[0][1] = r0[5];
			inv.m[0][2] = r0[6]; inv.m[0][3] = r0[7];
			inv.m[1][0] = r1[4]; inv.m[1][1] = r1[5];
			inv.m[1][2] = r1[6]; inv.m[1][3] = r1[7];
			inv.m[2][0] = r2[4]; inv.m[2][1] = r2[5];
			inv.m[2][2] = r2[6]; inv.m[2][3] = r2[7];
			inv.m[3][0] = r3[4]; inv.m[3][1] = r3[5];
			inv.m[3][2] = r3[6]; inv.m[3][3] = r3[7];

			return inv;
		}

		return DynMat::Zero( rows, columns );
	}

	const T* operator[]( size_t index ) const { return m[index]; }
	T* operator[]( size_t index ) { return m[index]; }

public:
	T m[MaxRows][MaxColumns];
	size_t rows;
	size_t columns;
};

template <size_t Rows, size_t Columns, size_t OtherRows, size_t OtherColumns>
inline DynMat<Rows, OtherColumns> operator*( const Mat<Rows, Columns>& lhs, const DynMat<OtherRows, OtherColumns>& rhs )
{
	assert( Columns == rhs.rows && "Invalid matrix dimensions for multiplication" );

	DynMat<Rows, OtherColumns> result( Rows, rhs.columns );
	for ( size_t y = 0; y < Rows; y++ )
	{
		for ( size_t x = 0; x < rhs.columns; x++ )
		{
			result[y][x] = 0.0f;
			for ( size_t i = 0; i < Columns; i++ )
			{
				result[y][x] += lhs[y][i] * rhs[i][x];
			}
		}
	}
	return result;
}

template <size_t Rows, size_t Columns, size_t OtherRows, size_t OtherColumns>
inline Mat<Rows, Columns> operator+( const Mat<Rows, Columns>& lhs, const DynMat<OtherRows, OtherColumns>& rhs )
{
	assert( Columns == rhs.columns && "Invalid matrix dimensions for addition" );
	assert( Rows == rhs.rows && "Invalid matrix dimensions for addition" );

	Mat<Rows, Columns> result;
	for ( size_t y = 0; y < Rows; y++ )
	{
		for ( size_t x = 0; x < Columns; x++ )
		{
			result[y][x] = lhs[y][x] + rhs[y][x];
		}
	}
	return result;
}

template <size_t Rows, size_t Columns, size_t OtherRows, size_t OtherColumns>
inline Mat<Rows, Columns> operator-( const Mat<Rows, Columns>& lhs, const DynMat<OtherRows, OtherColumns>& rhs )
{
	assert( Columns == rhs.columns && "Invalid matrix dimensions for addition" );
	assert( Rows == rhs.rows && "Invalid matrix dimensions for addition" );

	Mat<Rows, Columns> result;
	for ( size_t y = 0; y < Rows; y++ )
	{
		for ( size_t x = 0; x < Columns; x++ )
		{
			result[y][x] = lhs[y][x] - rhs[y][x];
		}
	}
	return result;
}

using Mat6x6 = Mat<6, 6>;
using Mat6x1 = Mat<6, 1>;