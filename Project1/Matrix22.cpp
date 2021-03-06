#include "Precompiled.h"
#include "Matrix22.h"
#include <iostream>			// ostream
#include <cmath>			// trig functions
#include <cstring>			// memcpy
#include "Vector2.h"        // 3D vector

Matrix22 const g_identity = Matrix22( 1, 0,
				                      0, 1 );

/*------------------------------------------------------------------------------
copy constructor
------------------------------------------------------------------------------*/
Matrix22::Matrix22( Matrix22 const &m ) {
	memcpy( data, m.data, sizeof( data ) );
}

/*------------------------------------------------------------------------------
conversion constructor
------------------------------------------------------------------------------*/
Matrix22::Matrix22( float const *f ) {
	*this = *(Matrix22 const*)f;
}

/*------------------------------------------------------------------------------
constructor
------------------------------------------------------------------------------*/
Matrix22::Matrix22( float _00, float _01,
			        float _10, float _11 ) {
	m._00 = _00;
	m._01 = _01;

	m._10 = _10;
	m._11 = _11;
}

/*------------------------------------------------------------------------------
constructor
------------------------------------------------------------------------------*/
Matrix22::Matrix22( Vector2 const &row0, Vector2 const &row1 ) {
    memcpy( data[ 0 ], row0, 3 * sizeof( float ) );
    memcpy( data[ 1 ], row1, 3 * sizeof( float ) );
}

/*------------------------------------------------------------------------------
accessor
------------------------------------------------------------------------------*/
float& Matrix22::operator()( uint32_t row, uint32_t col ) {
	return data[ row ][ col ];
}

/*------------------------------------------------------------------------------
accessor
------------------------------------------------------------------------------*/
float Matrix22::operator()( uint32_t row, uint32_t col ) const {
	return data[ row ][ col ];
}

/*------------------------------------------------------------------------------
accessor
------------------------------------------------------------------------------*/
float const* Matrix22::operator()( uint32_t row ) const {
    return data[ row ];
}

/*------------------------------------------------------------------------------
casting operator
------------------------------------------------------------------------------*/
Matrix22::operator float*() {
	return (float*)this;
}

/*------------------------------------------------------------------------------
casting operator
------------------------------------------------------------------------------*/
Matrix22::operator float const*() const {
	return (float const*)this;
}

/*------------------------------------------------------------------------------
assignment operator
------------------------------------------------------------------------------*/
Matrix22& Matrix22::operator*=( Matrix22 const &rhs ) {
	Matrix22 product;

    product.m._00 = data[ 0 ][ 0 ] * rhs.data[ 0 ][ 0 ] + data[ 0 ][ 1 ] * rhs.data[ 1 ][ 0 ];
    product.m._01 = data[ 0 ][ 0 ] * rhs.data[ 0 ][ 1 ] + data[ 0 ][ 1 ] * rhs.data[ 1 ][ 1 ];
    product.m._10 = data[ 1 ][ 0 ] * rhs.data[ 0 ][ 0 ] + data[ 1 ][ 1 ] * rhs.data[ 1 ][ 0 ];
    product.m._11 = data[ 1 ][ 0 ] * rhs.data[ 0 ][ 1 ] + data[ 1 ][ 1 ] * rhs.data[ 1 ][ 1 ];

	*this = product;
	return *this;
}

/*------------------------------------------------------------------------------
assignment operator
------------------------------------------------------------------------------*/
Matrix22& Matrix22::operator+=( Matrix22 const &rhs ) {
    m._00 += rhs.m._00;
    m._01 += rhs.m._01;
    m._10 += rhs.m._10;
    m._11 += rhs.m._11;
	return *this;
}

/*------------------------------------------------------------------------------
assignment operator
------------------------------------------------------------------------------*/
Matrix22& Matrix22::operator-=( Matrix22 const &rhs ) {
    m._00 -= rhs.m._00;
    m._01 -= rhs.m._01;
    m._10 -= rhs.m._10;
    m._11 -= rhs.m._11;
	return *this;
}

/*------------------------------------------------------------------------------
assignment operator
------------------------------------------------------------------------------*/
Matrix22& Matrix22::operator*=( float f ) {
    m._00 *= f;
    m._01 *= f;
    m._10 *= f;
    m._11 *= f;
	return *this;
}

/*------------------------------------------------------------------------------
assignment operator
------------------------------------------------------------------------------*/
Matrix22& Matrix22::operator/=(float f){
    m._00 /= f;
    m._01 /= f;
    m._10 /= f;
    m._11 /= f;
	return *this;
}

/*------------------------------------------------------------------------------
unary operator
------------------------------------------------------------------------------*/
Matrix22 Matrix22::operator+() const {
	return Matrix22( *this );
}

/*------------------------------------------------------------------------------
unary operator
------------------------------------------------------------------------------*/
Matrix22 Matrix22::operator-() const {
	return Matrix22( *this ) *= -1;
}

/*------------------------------------------------------------------------------
binary operator
------------------------------------------------------------------------------*/
Matrix22 Matrix22::operator*( Matrix22 const &rhs ) const {
	return Matrix22( *this ) *= rhs;
}

/*------------------------------------------------------------------------------
binary operator
------------------------------------------------------------------------------*/
Matrix22 Matrix22::operator+( Matrix22 const &rhs ) const {
	return Matrix22( *this ) += rhs;
}

/*------------------------------------------------------------------------------
binary operator
------------------------------------------------------------------------------*/
Matrix22 Matrix22::operator-( Matrix22 const &rhs ) const {
	return Matrix22( *this ) -= rhs;
}

/*------------------------------------------------------------------------------
binary operator
------------------------------------------------------------------------------*/
Matrix22 Matrix22::operator*( float f ) const {
	return Matrix22( *this ) *= f;
}

/*------------------------------------------------------------------------------
binary operator
------------------------------------------------------------------------------*/
Matrix22 Matrix22::operator/( float f ) const {
	return Matrix22( *this ) /= f;
}

/*------------------------------------------------------------------------------
friend binary operator
------------------------------------------------------------------------------*/
Matrix22 operator*( float f, Matrix22 const &rhs ) {
	return Matrix22( rhs ) *= f;
}

/*------------------------------------------------------------------------------
comparison operator
------------------------------------------------------------------------------*/
bool Matrix22::operator==( Matrix22 const &rhs ) const {
    return m._00 == rhs.m._00 &&
           m._01 == rhs.m._01 &&
           m._10 == rhs.m._10 &&
           m._11 == rhs.m._11;
}

/*------------------------------------------------------------------------------
comparison operator
------------------------------------------------------------------------------*/
bool Matrix22::operator!=( Matrix22 const &rhs ) const {
	return !( ( *this ) == rhs );
}

/*------------------------------------------------------------------------------
stream operator
------------------------------------------------------------------------------*/
std::ostream& operator<<( std::ostream &os, Matrix22 const &m ) {
	os << "[" << m.m._00 << " " << m.m._01 << " " << '\n' <<
		  " " << m.m._10 << " " << m.m._11 << " " << ']';
	return os;
}

/*------------------------------------------------------------------------------
set this to the identity matrix
------------------------------------------------------------------------------*/
Matrix22& Matrix22::SetToIdentity(){
	*this = g_identity;
	return *this;
}

/*------------------------------------------------------------------------------
set this to its transpose
------------------------------------------------------------------------------*/
Matrix22& Matrix22::Transpose(){
	*this = Matrix22Transpose( *this );
	return *this;
}

/*------------------------------------------------------------------------------
create an identity matrix
------------------------------------------------------------------------------*/
Matrix22 Matrix22Identity(){
	return g_identity;
}

/*------------------------------------------------------------------------------
transpose a matrix
------------------------------------------------------------------------------*/
Matrix22 Matrix22Transpose( Matrix22 const &m ){
	return Matrix22( m.m._00, m.m._10,
				     m.m._01, m.m._11 );
}

/*------------------------------------------------------------------------------
component-wise absolute value
------------------------------------------------------------------------------*/
Matrix22 Matrix22Abs( Matrix22 const &m ) {
    return Matrix22( fabs( m.m._00 ), fabs( m.m._01 ),
				     fabs( m.m._10 ), fabs( m.m._11 ) );
}

/*------------------------------------------------------------------------------
create a rotation matrix (angle is in radians)
------------------------------------------------------------------------------*/
Matrix22 Matrix22Rotation( float angle ) {
	float c = cosf( angle );
	float s = sinf( angle );
	return Matrix22( c, -s,
				     s,  c );
}

/*------------------------------------------------------------------------------
create a scaling matrix
------------------------------------------------------------------------------*/
Matrix22 Matrix22Scaling( float s ) {
	return Matrix22( s, 0,
				     0, s );
}

/*------------------------------------------------------------------------------
create a scaling matrix
------------------------------------------------------------------------------*/
Matrix22 Matrix33Scaling( float sx, float sy ) {
	return Matrix22( sx, 0,
				     0,  sy );
}
