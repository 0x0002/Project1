#include "Precompiled.h"
#include "Matrix33.h"
#include <iostream>			// ostream
#include <cmath>			// trig functions
#include <cstring>			// memcpy
#include "Vector3.h"        // 3D vector
#include "Matrix.h"         // 4x4 matrix
#include "Quaternion.h"     // Quaternion

Matrix33 const g_identity = Matrix33( 1, 0, 0,
				                      0, 1, 0,
				                      0, 0, 1 );

/*------------------------------------------------------------------------------
copy constructor
------------------------------------------------------------------------------*/
Matrix33::Matrix33( Matrix33 const &m ) {
	memcpy( data, m.data, sizeof( data ) );
}

/*------------------------------------------------------------------------------
conversion constructor
------------------------------------------------------------------------------*/
Matrix33::Matrix33( float const *f ) {
	*this = *(Matrix33 const*)f;
}

/*------------------------------------------------------------------------------
constructor
------------------------------------------------------------------------------*/
Matrix33::Matrix33( float _00, float _01, float _02,
			        float _10, float _11, float _12,
			        float _20, float _21, float _22 ) {
	m._00 = _00;
	m._01 = _01;
	m._02 = _02;

	m._10 = _10;
	m._11 = _11;
	m._12 = _12;

	m._20 = _20;
	m._21 = _21;
	m._22 = _22;
}

/*------------------------------------------------------------------------------
constructor
------------------------------------------------------------------------------*/
Matrix33::Matrix33( Vector3 const &row0, Vector3 const &row1, Vector3 const &row2 ) {
    memcpy( data[ 0 ], row0, 3 * sizeof( float ) );
    memcpy( data[ 1 ], row1, 3 * sizeof( float ) );
    memcpy( data[ 2 ], row2, 3 * sizeof( float ) );
}

/*------------------------------------------------------------------------------
constructor
------------------------------------------------------------------------------*/
Matrix33::Matrix33( Matrix const &m ) {
    memcpy( data[ 0 ], m( 0 ), 3 * sizeof( float ) );
    memcpy( data[ 1 ], m( 1 ), 3 * sizeof( float ) );
    memcpy( data[ 2 ], m( 2 ), 3 * sizeof( float ) );
}

/*------------------------------------------------------------------------------
accessor
------------------------------------------------------------------------------*/
float& Matrix33::operator()( uint32_t row, uint32_t col ) {
	return data[ row ][ col ];
}

/*------------------------------------------------------------------------------
accessor
------------------------------------------------------------------------------*/
float Matrix33::operator()( uint32_t row, uint32_t col ) const {
	return data[ row ][ col ];
}

/*------------------------------------------------------------------------------
accessor
------------------------------------------------------------------------------*/
float const* Matrix33::operator()( uint32_t row ) const {
    return data[ row ];
}

/*------------------------------------------------------------------------------
casting operator
------------------------------------------------------------------------------*/
Matrix33::operator float*() {
	return (float*)this;
}

/*------------------------------------------------------------------------------
casting operator
------------------------------------------------------------------------------*/
Matrix33::operator float const*() const {
	return (float const*)this;
}

/*------------------------------------------------------------------------------
assignment operator
------------------------------------------------------------------------------*/
Matrix33& Matrix33::operator*=( Matrix33 const &rhs ) {
	Matrix33 product;

	for( int r = 0; r < 3; ++r ) {
		for( int c = 0; c < 3; ++c ) {
			product.data[ r ][ c ] = data[ r ][ 0 ] * rhs.data[ 0 ][ c ] +
									 data[ r ][ 1 ] * rhs.data[ 1 ][ c ] +
									 data[ r ][ 2 ] * rhs.data[ 2 ][ c ];
		}
	}
	*this = product;
	return *this;
}

/*------------------------------------------------------------------------------
assignment operator
------------------------------------------------------------------------------*/
Matrix33& Matrix33::operator+=( Matrix33 const &rhs ) {
	for( int r = 0; r < 3; ++r ) {
		for( int c = 0; c < 3; ++c ) {
			data[ r ][ c ] += rhs.data[ r ][ c ];
		}
	}
	return *this;
}

/*------------------------------------------------------------------------------
assignment operator
------------------------------------------------------------------------------*/
Matrix33& Matrix33::operator-=( Matrix33 const &rhs ) {
	for( int r = 0; r < 3; ++r ) {
		for( int c = 0; c < 3; ++c ) {
			data[ r ][ c ] -= rhs.data[ r ][ c ];
		}
	}
	return *this;
}

/*------------------------------------------------------------------------------
assignment operator
------------------------------------------------------------------------------*/
Matrix33& Matrix33::operator*=( float f ) {
	for( int r = 0; r < 3; ++r ) {
		for( int c = 0; c < 3; ++c ) {
			data[ r ][ c ] *= f;
		}
	}
	return *this;
}

/*------------------------------------------------------------------------------
assignment operator
------------------------------------------------------------------------------*/
Matrix33& Matrix33::operator/=(float f){
	for( int r = 0; r < 3; ++r ) {
		for( int c = 0; c < 3; ++c ) {
			data[ r ][ c ] /= f;
		}
	}
	return *this;
}

/*------------------------------------------------------------------------------
unary operator
------------------------------------------------------------------------------*/
Matrix33 Matrix33::operator+() const {
	return Matrix33( *this );
}

/*------------------------------------------------------------------------------
unary operator
------------------------------------------------------------------------------*/
Matrix33 Matrix33::operator-() const {
	return Matrix33( *this ) *= -1;
}

/*------------------------------------------------------------------------------
binary operator
------------------------------------------------------------------------------*/
Matrix33 Matrix33::operator*( Matrix33 const &rhs ) const {
	return Matrix33( *this ) *= rhs;
}

/*------------------------------------------------------------------------------
binary operator
------------------------------------------------------------------------------*/
Matrix33 Matrix33::operator+( Matrix33 const &rhs ) const {
	return Matrix33( *this ) += rhs;
}

/*------------------------------------------------------------------------------
binary operator
------------------------------------------------------------------------------*/
Matrix33 Matrix33::operator-( Matrix33 const &rhs ) const {
	return Matrix33( *this ) -= rhs;
}

/*------------------------------------------------------------------------------
binary operator
------------------------------------------------------------------------------*/
Matrix33 Matrix33::operator*( float f ) const {
	return Matrix33( *this ) *= f;
}

/*------------------------------------------------------------------------------
binary operator
------------------------------------------------------------------------------*/
Matrix33 Matrix33::operator/( float f ) const {
	return Matrix33( *this ) /= f;
}

/*------------------------------------------------------------------------------
friend binary operator
------------------------------------------------------------------------------*/
Matrix33 operator*( float f, Matrix33 const &rhs ) {
	return Matrix33( rhs ) *= f;
}

/*------------------------------------------------------------------------------
comparison operator
------------------------------------------------------------------------------*/
bool Matrix33::operator==( Matrix33 const &rhs ) const {
	for( int r = 0; r < 3; ++r ) {
		for( int c = 0; c < 3; ++c ) {
			if( data[ r ][ c ] != rhs.data[ r ][ c ] ) return false;
		}
	}
	return true;
}

/*------------------------------------------------------------------------------
comparison operator
------------------------------------------------------------------------------*/
bool Matrix33::operator!=( Matrix33 const &rhs ) const {
	return !( ( *this ) == rhs );
}

/*------------------------------------------------------------------------------
stream operator
------------------------------------------------------------------------------*/
std::ostream& operator<<( std::ostream &os, Matrix33 const &m ) {
	os << "[" << m.m._00 << " " << m.m._01 << " " << m.m._02 << '\n' <<
		  " " << m.m._10 << " " << m.m._11 << " " << m.m._12 << '\n' <<
		  " " << m.m._20 << " " << m.m._21 << " " << m.m._22 << ']';
	return os;
}

/*------------------------------------------------------------------------------
set this to the identity matrix
------------------------------------------------------------------------------*/
Matrix33& Matrix33::SetToIdentity(){
	*this = g_identity;
	return *this;
}

/*------------------------------------------------------------------------------
set this to its transpose
------------------------------------------------------------------------------*/
Matrix33& Matrix33::Transpose(){
	*this = Matrix33Transpose( *this );
	return *this;
}

/*------------------------------------------------------------------------------
create an identity matrix
------------------------------------------------------------------------------*/
Matrix33 Matrix33Identity(){
	return g_identity;
}

/*------------------------------------------------------------------------------
transpose a matrix
------------------------------------------------------------------------------*/
Matrix33 Matrix33Transpose( Matrix33 const &m ){
	return Matrix33( m.m._00, m.m._10, m.m._20,
				     m.m._01, m.m._11, m.m._21,
				     m.m._02, m.m._12, m.m._22 );
}

/*------------------------------------------------------------------------------
component-wise absolute value
------------------------------------------------------------------------------*/
Matrix33 Matrix33Abs( Matrix33 const &m ) {
    return Matrix33( fabs( m.m._00 ), fabs( m.m._01 ), fabs( m.m._02 ),
                     fabs( m.m._10 ), fabs( m.m._11 ), fabs( m.m._12 ),
                     fabs( m.m._20 ), fabs( m.m._21 ), fabs( m.m._22 ) );
}

/*------------------------------------------------------------------------------
create a rotation matrix (angle is in radians)
------------------------------------------------------------------------------*/
Matrix33 Matrix33RotationX( float angle ) {
	float c = cosf( angle );
	float s = sinf( angle );
	return Matrix33( 1,  0,  0,
				     0,  c,  s,
				     0, -s,  c );
}

/*------------------------------------------------------------------------------
create a rotation matrix (angle is in radians)
------------------------------------------------------------------------------*/
Matrix33 Matrix33RotationY( float angle ) {
	float c = cosf( angle );
	float s = sinf( angle );
	return Matrix33( c,  0, -s,
				     0,  1,  0,
				     s,  0,  c );
}

/*------------------------------------------------------------------------------
create a rotation matrix (angle is in radians)
------------------------------------------------------------------------------*/
Matrix33 Matrix33RotationZ( float angle ) {
	float c = cosf( angle );
	float s = sinf( angle );
	return Matrix33(  c,  s,  0,
				     -s,  c,  0,
				      0,  0,  1 );
}

/*------------------------------------------------------------------------------
create a rotation matrix (angle is in radians)
------------------------------------------------------------------------------*/
Matrix33 Matrix33RotationYawPitchRoll( float yaw, float pitch, float roll ) {
	return Matrix33RotationZ( roll ) * Matrix33RotationX( pitch ) * Matrix33RotationY( yaw );
}

/*------------------------------------------------------------------------------
create a rotation matrix (angle is in radians)
------------------------------------------------------------------------------*/
Matrix33 Matrix33RotationAxisAngle( Vector3 const &axis, float angle ) {
	float c = cosf( angle );
	float s = sinf( angle );
	float t = 1 - c;

	Vector3 normalizedAxis = Normalize( axis );
	float const &x = normalizedAxis.x;
	float const &y = normalizedAxis.y;
	float const &z = normalizedAxis.z;

	return Matrix33( t * x * x + c,     t * x * y + s * z, t * x * z - s * y,
				     t * x * y - s * z, t * y * y + c,     t * y * z + s * x,
				     t * x * z + s * y, t * y * z - s * x, t * z * z + c      );
}

/*------------------------------------------------------------------------------
create a rotation matrix from a quaternion
the quaternion should be normalized
------------------------------------------------------------------------------*/
Matrix33 Matrix33RotationQuaternion( Quaternion const &q ) {
	// premultiply terms
	float xx = q.x * q.x;
	float xy = q.x * q.y;
	float xz = q.x * q.z;
	float xw = q.x * q.w;
	float yy = q.y * q.y;
	float yz = q.y * q.z;
	float yw = q.y * q.w;
	float zz = q.z * q.z;
	float zw = q.z * q.w;

	return Matrix33( 1 - 2 * ( yy + zz ), 2 * ( xy + zw ),     2 * ( xz - yw ),
				     2 * ( xy - zw ),     1 - 2 * ( xx + zz ), 2 * ( yz + xw ),
				     2 * ( xz + yw ),     2 * ( yz - xw ),     1 - 2 * ( xx + yy ) );
}

/*------------------------------------------------------------------------------
create a scaling matrix
------------------------------------------------------------------------------*/
Matrix33 Matrix33Scaling( float s ) {
	return Matrix33( s, 0, 0,
				     0, s, 0,
				     0, 0, s );
}

/*------------------------------------------------------------------------------
create a scaling matrix
------------------------------------------------------------------------------*/
Matrix33 Matrix33Scaling( float sx, float sy, float sz ) {
	return Matrix33( sx, 0,  0,
				     0,  sy, 0,
				     0,  0,  sz );
}
