#include "Precompiled.h"
#include "Matrix.h"
#include <iostream>			// ostream
#include <cmath>			// trig functions
#include <cstring>			// memcpy
#include "Vector3.h"		// 3D vector
#include "Vector4.h"        // 4D vector
#include "Matrix33.h"       // 3x3 matrix
#include "Quaternion.h"		// Quaternion

Matrix const g_identity = Matrix( 1, 0, 0, 0,
				                  0, 1, 0, 0,
				                  0, 0, 1, 0,
				                  0, 0, 0, 1 );

/*------------------------------------------------------------------------------
copy constructor
------------------------------------------------------------------------------*/
Matrix::Matrix( Matrix const &m ) {
	memcpy( data, m.data, sizeof( data ) );
}

/*------------------------------------------------------------------------------
conversion constructor
------------------------------------------------------------------------------*/
Matrix::Matrix( float const *f ) {
	*this = *(Matrix const*)f;
}

/*------------------------------------------------------------------------------
constructor
------------------------------------------------------------------------------*/
Matrix::Matrix( float _00, float _01, float _02, float _03,
			    float _10, float _11, float _12, float _13,
			    float _20, float _21, float _22, float _23,
			    float _30, float _31, float _32, float _33 ) {
	m._00 = _00;
	m._01 = _01;
	m._02 = _02;
	m._03 = _03;

	m._10 = _10;
	m._11 = _11;
	m._12 = _12;
	m._13 = _13;

	m._20 = _20;
	m._21 = _21;
	m._22 = _22;
	m._23 = _23;

	m._30 = _30;
	m._31 = _31;
	m._32 = _32;
	m._33 = _33;
}

/*------------------------------------------------------------------------------
constructor
------------------------------------------------------------------------------*/
Matrix::Matrix( Vector4 const &row0,
                Vector4 const &row1,
                Vector4 const &row2,
                Vector4 const &row3 ) {
    memcpy( data[ 0 ], row0, 4 * sizeof( float ) );
    memcpy( data[ 1 ], row1, 4 * sizeof( float ) );
    memcpy( data[ 2 ], row2, 4 * sizeof( float ) );
    memcpy( data[ 3 ], row3, 4 * sizeof( float ) );
}

/*------------------------------------------------------------------------------
constructor
------------------------------------------------------------------------------*/
Matrix::Matrix( Matrix33 const &rotation, Vector3 const &translation ) {
    memcpy( data[ 0 ], rotation( 0 ), 3 * sizeof( float ) );
    memcpy( data[ 1 ], rotation( 0 ), 3 * sizeof( float ) );
    memcpy( data[ 2 ], rotation( 0 ), 3 * sizeof( float ) );
    memcpy( data[ 3 ], translation,   3 * sizeof( float ) );
    m._03 = m._13 = m._23 = 0.0f;
    m._33 = 1.0f;
}

/*------------------------------------------------------------------------------
accessor
------------------------------------------------------------------------------*/
float& Matrix::operator()( uint32_t row, uint32_t col ) {
	return data[ row ][ col ];
}

/*------------------------------------------------------------------------------
accessor
------------------------------------------------------------------------------*/
float Matrix::operator()( uint32_t row, uint32_t col ) const {
	return data[ row ][ col ];
}

/*------------------------------------------------------------------------------
accessor
------------------------------------------------------------------------------*/
float const* Matrix::operator()( uint32_t row ) const {
	return data[ row ];
}

/*------------------------------------------------------------------------------
casting operator
------------------------------------------------------------------------------*/
Matrix::operator float*() {
	return (float*)this;
}

/*------------------------------------------------------------------------------
casting operator
------------------------------------------------------------------------------*/
Matrix::operator float const*() const {
	return (float const*)this;
}

/*------------------------------------------------------------------------------
assignment operator
------------------------------------------------------------------------------*/
Matrix& Matrix::operator*=( Matrix const &rhs ) {
	Matrix product;

	for( int r = 0; r < 4; ++r ) {
		for( int c = 0; c < 4; ++c ) {
			product.data[ r ][ c ] = data[ r ][ 0 ] * rhs.data[ 0 ][ c ] +
									 data[ r ][ 1 ] * rhs.data[ 1 ][ c ] +
									 data[ r ][ 2 ] * rhs.data[ 2 ][ c ] +
									 data[ r ][ 3 ] * rhs.data[ 3 ][ c ];
		}
	}
	*this = product;
	return *this;
}

/*------------------------------------------------------------------------------
assignment operator
------------------------------------------------------------------------------*/
Matrix& Matrix::operator+=( Matrix const &rhs ) {
	for( int r = 0; r < 4; ++r ) {
		for( int c = 0; c < 4; ++c ) {
			data[ r ][ c ] += rhs.data[ r ][ c ];
		}
	}
	return *this;
}

/*------------------------------------------------------------------------------
assignment operator
------------------------------------------------------------------------------*/
Matrix& Matrix::operator-=( Matrix const &rhs ) {
	for( int r = 0; r < 4; ++r ) {
		for( int c = 0; c < 4; ++c ) {
			data[ r ][ c ] -= rhs.data[ r ][ c ];
		}
	}
	return *this;
}

/*------------------------------------------------------------------------------
assignment operator
------------------------------------------------------------------------------*/
Matrix& Matrix::operator*=( float f ) {
	for( int r = 0; r < 4; ++r ) {
		for( int c = 0; c < 4; ++c ) {
			data[ r ][ c ] *= f;
		}
	}
	return *this;
}

/*------------------------------------------------------------------------------
assignment operator
------------------------------------------------------------------------------*/
Matrix& Matrix::operator/=(float f){
	for( int r = 0; r < 4; ++r ) {
		for( int c = 0; c < 4; ++c ) {
			data[ r ][ c ] /= f;
		}
	}
	return *this;
}

/*------------------------------------------------------------------------------
unary operator
------------------------------------------------------------------------------*/
Matrix Matrix::operator+() const {
	return Matrix( *this );
}

/*------------------------------------------------------------------------------
unary operator
------------------------------------------------------------------------------*/
Matrix Matrix::operator-() const {
	return Matrix( *this ) *= -1;
}

/*------------------------------------------------------------------------------
binary operator
------------------------------------------------------------------------------*/
Matrix Matrix::operator*( Matrix const &rhs ) const {
	return Matrix( *this ) *= rhs;
}

/*------------------------------------------------------------------------------
binary operator
------------------------------------------------------------------------------*/
Matrix Matrix::operator+( Matrix const &rhs ) const {
	return Matrix( *this ) += rhs;
}

/*------------------------------------------------------------------------------
binary operator
------------------------------------------------------------------------------*/
Matrix Matrix::operator-( Matrix const &rhs ) const {
	return Matrix( *this ) -= rhs;
}

/*------------------------------------------------------------------------------
binary operator
------------------------------------------------------------------------------*/
Matrix Matrix::operator*( float f ) const {
	return Matrix( *this ) *= f;
}

/*------------------------------------------------------------------------------
binary operator
------------------------------------------------------------------------------*/
Matrix Matrix::operator/( float f ) const {
	return Matrix( *this ) /= f;
}

/*------------------------------------------------------------------------------
friend binary operator
------------------------------------------------------------------------------*/
Matrix operator*( float f, Matrix const &rhs ) {
	return Matrix( rhs ) *= f;
}

/*------------------------------------------------------------------------------
comparison operator
------------------------------------------------------------------------------*/
bool Matrix::operator==( Matrix const &rhs ) const {
	for( int r = 0; r < 4; ++r ) {
		for( int c = 0; c < 4; ++c ) {
			if( data[ r ][ c ] != rhs.data[ r ][ c ] ) return false;
		}
	}
	return true;
}

/*------------------------------------------------------------------------------
comparison operator
------------------------------------------------------------------------------*/
bool Matrix::operator!=( Matrix const &rhs ) const {
	return !( ( *this ) == rhs );
}

/*------------------------------------------------------------------------------
stream operator
------------------------------------------------------------------------------*/
std::ostream& operator<<( std::ostream &os, Matrix const &m ) {
	os << "[" << m.m._00 << " " << m.m._01 << " " << m.m._02 << " " << m.m._03 << '\n' <<
		  " " << m.m._10 << " " << m.m._11 << " " << m.m._12 << " " << m.m._13 << '\n' <<
		  " " << m.m._20 << " " << m.m._21 << " " << m.m._22 << " " << m.m._23 << '\n' <<
		  " " << m.m._30 << " " << m.m._31 << " " << m.m._32 << " " << m.m._33 << ']';
	return os;
}

/*------------------------------------------------------------------------------
set this to the identity matrix
------------------------------------------------------------------------------*/
Matrix& Matrix::SetToIdentity(){
	*this = g_identity;
	return *this;
}

/*------------------------------------------------------------------------------
set this to its transpose
------------------------------------------------------------------------------*/
Matrix& Matrix::Transpose(){
	*this = MatrixTranspose( *this );
	return *this;
}

/*------------------------------------------------------------------------------
set this to its inverse
returns the determinant
if the determinant is 0, the matrix is not invertible,
and is the matrix is not changed
------------------------------------------------------------------------------*/
float Matrix::Invert(){
	float determinant;
	*this = MatrixInverse( *this, &determinant );
	return determinant;
}

/*------------------------------------------------------------------------------
create an identity matrix
------------------------------------------------------------------------------*/
Matrix MatrixIdentity(){
	return g_identity;
}

/*------------------------------------------------------------------------------
transpose a matrix
------------------------------------------------------------------------------*/
Matrix MatrixTranspose( Matrix const &m ){
	return Matrix( m.m._00, m.m._10, m.m._20, m.m._30,
				   m.m._01, m.m._11, m.m._21, m.m._31,
				   m.m._02, m.m._12, m.m._22, m.m._32,
				   m.m._03, m.m._13, m.m._23, m.m._33 );
}

/*------------------------------------------------------------------------------
component-wise absolute value
------------------------------------------------------------------------------*/
Matrix MatrixAbs( Matrix const &m ) {
    return Matrix( fabs( m.m._00 ), fabs( m.m._01 ), fabs( m.m._02 ), fabs( m.m._03 ),
                   fabs( m.m._10 ), fabs( m.m._11 ), fabs( m.m._12 ), fabs( m.m._13 ),
                   fabs( m.m._20 ), fabs( m.m._21 ), fabs( m.m._22 ), fabs( m.m._23 ),
                   fabs( m.m._30 ), fabs( m.m._31 ), fabs( m.m._32 ), fabs( m.m._33 ) );
}

/*------------------------------------------------------------------------------
invert a matrix
returns the determinant if the argument is not NULL
if the matrix is not invertible, the determinant will be zero,
and the un-inverted matrix is returned

based on Cramer's rule and code from an Intel paper called
"Streaming SIMD Extensions - Inverse of 4x4 Matrix"
------------------------------------------------------------------------------*/
Matrix MatrixInverse( Matrix const &m, float *determinant ){

	float const *original = m;		// cast the matrix as a flat array
    float temp[ 12 ];				// used for intermediate calculations
	float src[ 16 ];				// transpose of original matrix inverse
	float inverse[ 16 ];			// matrix inverse
    
	// transpose the original matrix
    for( int i =  0; i < 4; i++ ) {
        src[ i      ] = original[ i * 4    ];
        src[ i + 4  ] = original[ i * 4 + 1];
        src[ i + 8  ] = original[ i * 4 + 2];
        src[ i + 12 ] = original[ i * 4 + 3];
    }

	// calculate pairs for the first 8 elements (cofactors)
    temp[  0 ] = src[ 10 ] * src[ 15 ];
    temp[  1 ] = src[ 11 ] * src[ 14 ];
    temp[  2 ] = src[  9 ] * src[ 15 ];
    temp[  3 ] = src[ 11 ] * src[ 13 ];
    temp[  4 ] = src[  9 ] * src[ 14 ];
    temp[  5 ] = src[ 10 ] * src[ 13 ];
    temp[  6 ] = src[  8 ] * src[ 15 ];
    temp[  7 ] = src[ 11 ] * src[ 12 ];
    temp[  8 ] = src[  8 ] * src[ 14 ];
    temp[  9 ] = src[ 10 ] * src[ 12 ];
    temp[ 10 ] = src[  8 ] * src[ 13 ];
    temp[ 11 ] = src[  9 ] * src[ 12 ];

    // calculate first 8 elements (cofactors)
    inverse[ 0 ]  = temp[ 0 ] * src[ 5 ] + temp[ 3 ] * src[ 6 ] + temp[  4 ] * src[ 7 ];
    inverse[ 0 ] -= temp[ 1 ] * src[ 5 ] + temp[ 2 ] * src[ 6 ] + temp[  5 ] * src[ 7 ];
    inverse[ 1 ]  = temp[ 1 ] * src[ 4 ] + temp[ 6 ] * src[ 6 ] + temp[  9 ] * src[ 7 ];
    inverse[ 1 ] -= temp[ 0 ] * src[ 4 ] + temp[ 7 ] * src[ 6 ] + temp[  8 ] * src[ 7 ];
    inverse[ 2 ]  = temp[ 2 ] * src[ 4 ] + temp[ 7 ] * src[ 5 ] + temp[ 10 ] * src[ 7 ];
    inverse[ 2 ] -= temp[ 3 ] * src[ 4 ] + temp[ 6 ] * src[ 5 ] + temp[ 11 ] * src[ 7 ];
    inverse[ 3 ]  = temp[ 5 ] * src[ 4 ] + temp[ 8 ] * src[ 5 ] + temp[ 11 ] * src[ 6 ];
    inverse[ 3 ] -= temp[ 4 ] * src[ 4 ] + temp[ 9 ] * src[ 5 ] + temp[ 10 ] * src[ 6 ];
    inverse[ 4 ]  = temp[ 1 ] * src[ 1 ] + temp[ 2 ] * src[ 2 ] + temp[  5 ] * src[ 3 ];
    inverse[ 4 ] -= temp[ 0 ] * src[ 1 ] + temp[ 3 ] * src[ 2 ] + temp[  4 ] * src[ 3 ];
    inverse[ 5 ]  = temp[ 0 ] * src[ 0 ] + temp[ 7 ] * src[ 2 ] + temp[  8 ] * src[ 3 ];
    inverse[ 5 ] -= temp[ 1 ] * src[ 0 ] + temp[ 6 ] * src[ 2 ] + temp[  9 ] * src[ 3 ];
    inverse[ 6 ]  = temp[ 3 ] * src[ 0 ] + temp[ 6 ] * src[ 1 ] + temp[ 11 ] * src[ 3 ];
    inverse[ 6 ] -= temp[ 2 ] * src[ 0 ] + temp[ 7 ] * src[ 1 ] + temp[ 10 ] * src[ 3 ];
    inverse[ 7 ]  = temp[ 4 ] * src[ 0 ] + temp[ 9 ] * src[ 1 ] + temp[ 10 ] * src[ 2 ];
    inverse[ 7 ] -= temp[ 5 ] * src[ 0 ] + temp[ 8 ] * src[ 1 ] + temp[ 11 ] * src[ 2 ];

    // calculate pairs for second 8 elements (cofactors)
    temp[  0 ] = src[ 2 ] * src[ 7 ];
    temp[  1 ] = src[ 3 ] * src[ 6 ];
    temp[  2 ] = src[ 1 ] * src[ 7 ];
    temp[  3 ] = src[ 3 ] * src[ 5 ];
    temp[  4 ] = src[ 1 ] * src[ 6 ];
    temp[  5 ] = src[ 2 ] * src[ 5 ];
    temp[  6 ] = src[ 0 ] * src[ 7 ];
    temp[  7 ] = src[ 3 ] * src[ 4 ];
    temp[  8 ] = src[ 0 ] * src[ 6 ];
    temp[  9 ] = src[ 2 ] * src[ 4 ];
    temp[ 10 ] = src[ 0 ] * src[ 5 ];
    temp[ 11 ] = src[ 1 ] * src[ 4 ];

    // calculate second 8 elements (cofactors)
    inverse[  8 ]  = temp[  0 ] * src[ 13 ] + temp[  3 ] * src[ 14 ] + temp[  4 ] * src[ 15 ];
    inverse[  8 ] -= temp[  1 ] * src[ 13 ] + temp[  2 ] * src[ 14 ] + temp[  5 ] * src[ 15 ];
    inverse[  9 ]  = temp[  1 ] * src[ 12 ] + temp[  6 ] * src[ 14 ] + temp[  9 ] * src[ 15 ];
    inverse[  9 ] -= temp[  0 ] * src[ 12 ] + temp[  7 ] * src[ 14 ] + temp[  8 ] * src[ 15 ];
    inverse[ 10 ]  = temp[  2 ] * src[ 12 ] + temp[  7 ] * src[ 13 ] + temp[ 10 ] * src[ 15 ];
    inverse[ 10 ] -= temp[  3 ] * src[ 12 ] + temp[  6 ] * src[ 13 ] + temp[ 11 ] * src[ 15 ];
    inverse[ 11 ]  = temp[  5 ] * src[ 12 ] + temp[  8 ] * src[ 13 ] + temp[ 11 ] * src[ 14 ];
    inverse[ 11 ] -= temp[  4 ] * src[ 12 ] + temp[  9 ] * src[ 13 ] + temp[ 10 ] * src[ 14 ];
    inverse[ 12 ]  = temp[  2 ] * src[ 10 ] + temp[  5 ] * src[ 11 ] + temp[  1 ] * src[  9 ];
    inverse[ 12 ] -= temp[  4 ] * src[ 11 ] + temp[  0 ] * src[  9 ] + temp[  3 ] * src[ 10 ];
    inverse[ 13 ]  = temp[  8 ] * src[ 11 ] + temp[  0 ] * src[  8 ] + temp[  7 ] * src[ 10 ];
    inverse[ 13 ] -= temp[  6 ] * src[ 10 ] + temp[  9 ] * src[ 11 ] + temp[  1 ] * src[  8 ];
    inverse[ 14 ]  = temp[  6 ] * src[  9 ] + temp[ 11 ] * src[ 11 ] + temp[  3 ] * src[  8 ];
    inverse[ 14 ] -= temp[ 10 ] * src[ 11 ] + temp[  2 ] * src[  8 ] + temp[  7 ] * src[  9 ];
    inverse[ 15 ]  = temp[ 10 ] * src[ 10 ] + temp[  4 ] * src[  8 ] + temp[  9 ] * src[  9 ];
    inverse[ 15 ] -= temp[  8 ] * src[  9 ] + temp[ 11 ] * src[ 10 ] + temp[  5 ] * src[  8 ];

    // calculate determinant
    float det = src[ 0 ] * inverse[ 0 ] + src[ 1 ] * inverse[ 1 ] + src[ 2 ] * inverse[ 2 ] + src[ 3 ] * inverse[ 3 ];

	if( det == 0 ) {
		if( determinant != NULL ) *determinant = 0;
		return Matrix( m );
	}

	// calculate matrix inverse
	if( determinant != NULL ) *determinant = det;
    det =  1.0f / det;
	return Matrix( inverse ) * det;
}

/*------------------------------------------------------------------------------
create a translation matrix
------------------------------------------------------------------------------*/
Matrix MatrixTranslation( float tx, float ty, float tz ) {
	return Matrix( 1,  0,  0,  0,
				   0,  1,  0,  0,
				   0,  0,  1,  0,
				   tx, ty, tz, 1 );
}

/*------------------------------------------------------------------------------
create a translation matrix
------------------------------------------------------------------------------*/
Matrix MatrixTranslation( Vector3 const &p ) {
	return Matrix( 1,   0,   0,   0,
				   0,   1,   0,   0,
				   0,   0,   1,   0,
				   p.x, p.y, p.z, 1 );
}

/*------------------------------------------------------------------------------
create a rotation matrix (angle is in radians)
------------------------------------------------------------------------------*/
Matrix MatrixRotationX( float angle ) {
	float c = cosf( angle );
	float s = sinf( angle );
	return Matrix( 1,  0,  0,  0,
				   0,  c,  s,  0,
				   0, -s,  c,  0,
				   0,  0,  0,  1 );
}

/*------------------------------------------------------------------------------
create a rotation matrix (angle is in radians)
------------------------------------------------------------------------------*/
Matrix MatrixRotationY( float angle ) {
	float c = cosf( angle );
	float s = sinf( angle );
	return Matrix( c,  0, -s,  0,
				   0,  1,  0,  0,
				   s,  0,  c,  0,
				   0,  0,  0,  1 );
}

/*------------------------------------------------------------------------------
create a rotation matrix (angle is in radians)
------------------------------------------------------------------------------*/
Matrix MatrixRotationZ( float angle ) {
	float c = cosf( angle );
	float s = sinf( angle );
	return Matrix(  c,  s,  0,  0,
				   -s,  c,  0,  0,
				    0,  0,  1,  0,
				    0,  0,  0,  1 );
}

/*------------------------------------------------------------------------------
create a rotation matrix (angle is in radians)
------------------------------------------------------------------------------*/
Matrix MatrixRotationYawPitchRoll( float yaw, float pitch, float roll ) {
	return MatrixRotationZ( roll ) * MatrixRotationX( pitch ) * MatrixRotationY( yaw );
}

/*------------------------------------------------------------------------------
create a rotation matrix (angle is in radians)
------------------------------------------------------------------------------*/
Matrix MatrixRotationAxisAngle( Vector3 const &axis, float angle ) {
	float c = cosf( angle );
	float s = sinf( angle );
	float t = 1 - c;

	Vector3 normalizedAxis = Normalize( axis );
	float const &x = normalizedAxis.x;
	float const &y = normalizedAxis.y;
	float const &z = normalizedAxis.z;

	return Matrix( t * x * x + c,     t * x * y + s * z, t * x * z - s * y, 0,
				   t * x * y - s * z, t * y * y + c,     t * y * z + s * x, 0,
				   t * x * z + s * y, t * y * z - s * x, t * z * z + c,     0,
				   0,                 0,                 0,                 1 );
}

/*------------------------------------------------------------------------------
create a rotation matrix from a quaternion
the quaternion should be normalized
------------------------------------------------------------------------------*/
Matrix MatrixRotationQuaternion( Quaternion const &q ) {
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

	return Matrix( 1 - 2 * ( yy + zz ), 2 * ( xy + zw ),     2 * ( xz - yw ),     0,
				   2 * ( xy - zw ),     1 - 2 * ( xx + zz ), 2 * ( yz + xw ),     0,
				   2 * ( xz + yw ),     2 * ( yz - xw ),     1 - 2 * ( xx + yy ), 0,
				   0,                   0,                   0,                   1 );
}

/*------------------------------------------------------------------------------
create a scaling matrix
------------------------------------------------------------------------------*/
Matrix MatrixScaling( float s ) {
	return Matrix( s, 0, 0, 0,
				   0, s, 0, 0,
				   0, 0, s, 0,
				   0, 0, 0, 1 );
}

/*------------------------------------------------------------------------------
create a scaling matrix
------------------------------------------------------------------------------*/
Matrix MatrixScaling( float sx, float sy, float sz ) {
	return Matrix( sx, 0,  0,  0,
				   0,  sy, 0,  0,
				   0,  0,  sz, 0,
				   0,  0,  0,  1 );
}

/*------------------------------------------------------------------------------
create a transform matrix
------------------------------------------------------------------------------*/
Matrix MatrixVQS( Vector3 const &v, Quaternion const &q, float s ) {
    Matrix m = MatrixScaling( s ) * MatrixRotationQuaternion( q );
    m.m._30 = v.x;
    m.m._31 = v.y;
    m.m._32 = v.z;
    return m;
}

/*------------------------------------------------------------------------------
create a left-handed view matrix
------------------------------------------------------------------------------*/
Matrix MatrixLookAtLH( Vector3 const &eye, Vector3 const &at, Vector3 const &up ) {
	Vector3 zAxis = Normalize( at - eye );
	Vector3 xAxis = Normalize( Cross( up, zAxis ) );
	Vector3 yAxis = Cross( zAxis, xAxis );

	return Matrix( xAxis.x,            yAxis.x,            zAxis.x,            0,
				   xAxis.y,            yAxis.y,            zAxis.y,            0,
				   xAxis.z,            yAxis.z,            zAxis.z,            0,
				   -Dot( xAxis, eye ), -Dot( yAxis, eye ), -Dot( zAxis, eye ), 1 );
}

/*------------------------------------------------------------------------------
create an orthographic projection matrix
------------------------------------------------------------------------------*/
Matrix MatrixOrthographicLH( float width, float height, float zNear, float zFar ) {
	return Matrix( 2 / width, 0,          0,                         0,
				   0,         2 / height, 0,                         0,
				   0,         0,          1 / ( zFar - zNear ),      0,
				   0,         0,          -zNear / ( zFar - zNear ), 1 );
}

/*------------------------------------------------------------------------------
create a perspective projection matrix (fov is radians)
------------------------------------------------------------------------------*/
Matrix MatrixPerspectiveFovLH( float fov, float aspectRatio, float zNear, float zFar ) {
	float yScale = 1 / tan( fov / 2 );
	float xScale = yScale / aspectRatio;

	return Matrix( xScale, 0,      0,                                0,
				   0,      yScale, 0,                                0,
				   0,      0,      zFar / ( zFar - zNear ),          1,
				   0,      0,      -zNear * zFar / ( zFar - zNear ), 0);
}

/*------------------------------------------------------------------------------
create a right-handed view matrix
------------------------------------------------------------------------------*/
Matrix MatrixLookAtRH( Vector3 const &eye, Vector3 const &at, Vector3 const &up ) {
	Vector3 zAxis = Normalize( eye - at );
	Vector3 xAxis = Normalize( Cross( up, zAxis ) );
	Vector3 yAxis = Cross( zAxis, xAxis );

	return Matrix( xAxis.x,            yAxis.x,            zAxis.x,            0,
				   xAxis.y,            yAxis.y,            zAxis.y,            0,
				   xAxis.z,            yAxis.z,            zAxis.z,            0,
				   -Dot( xAxis, eye ), -Dot( yAxis, eye ), -Dot( zAxis, eye ), 1 );
}

/*------------------------------------------------------------------------------
create an orthographic projection matrix
------------------------------------------------------------------------------*/
Matrix MatrixOrthographicRH( float width, float height, float zNear, float zFar ) {
	return Matrix( 2 / width, 0,          0,                        0,
				   0,         2 / height, 0,                        0,
				   0,         0,          1 / ( zNear - zFar ),     0,
				   0,         0,          zNear / ( zNear - zFar ), 1 );
}

/*------------------------------------------------------------------------------
create a perspective projection matrix (fov is radians)
------------------------------------------------------------------------------*/
Matrix MatrixPerspectiveFovRH( float fov, float aspectRatio, float zNear, float zFar ) {
	float yScale = 1 / tan( fov / 2 );
	float xScale = yScale / aspectRatio;

	return Matrix( xScale, 0,      0,                                0,
				   0,      yScale, 0,                                0,
				   0,      0,      zFar / ( zNear - zFar ),         -1,
				   0,      0,      zNear * zFar / ( zNear - zFar ),  0);
}