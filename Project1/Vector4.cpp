#include "Precompiled.h"
#include "Vector4.h"
#include <iostream>			// ostream
#include <cmath>			// square root
#include "Vector2.h"		// 2d vector
#include "Vector3.h"		// 3d vector
#include "Matrix.h"			// 4x4 matrix

/*------------------------------------------------------------------------------
conversion constructor
------------------------------------------------------------------------------*/
Vector4::Vector4( Vector2 const &v, float z, float w ) :
	x( v.x ), y( v.y ), z( z ), w( w ) {
}

/*------------------------------------------------------------------------------
conversion constructor
------------------------------------------------------------------------------*/
Vector4::Vector4( Vector3 const &v, float w ) :
	x( v.x ), y( v.y ), z( v.z ), w( w ) {
}

/*------------------------------------------------------------------------------
copy constructor
------------------------------------------------------------------------------*/
Vector4::Vector4( Vector4 const &v ) :
	x( v.x ), y( v.y ), z( v.z ), w( v.w ) {
}

/*------------------------------------------------------------------------------
conversion constructor
------------------------------------------------------------------------------*/
Vector4::Vector4( float const *f  ) {
	*this = *(Vector4 const*)f;
}

/*------------------------------------------------------------------------------
constructor
------------------------------------------------------------------------------*/
Vector4::Vector4( float x, float y, float z, float w ) :
	x( x ), y( y ), z( z ), w( w ) {
}

/*------------------------------------------------------------------------------
casting operator
------------------------------------------------------------------------------*/
Vector4::operator float*() {
	return (float*)this;
}

/*------------------------------------------------------------------------------
casting operator
------------------------------------------------------------------------------*/
Vector4::operator float const*() const {
	return (float const*)this;
}

/*------------------------------------------------------------------------------
assignment operator
------------------------------------------------------------------------------*/
Vector4& Vector4::operator+=( Vector4 const &v ) {
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

/*------------------------------------------------------------------------------
assignment operator
------------------------------------------------------------------------------*/
Vector4& Vector4::operator-=( Vector4 const &v ) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;
}

/*------------------------------------------------------------------------------
assignment operator row vector * 4x4 matrix)
------------------------------------------------------------------------------*/
Vector4& Vector4::operator*=( Matrix const &m) {
	*this = *this * m;
	return *this;
}

/*------------------------------------------------------------------------------
assignment operator
------------------------------------------------------------------------------*/
Vector4& Vector4::operator*=( float f ) {
	x *= f;
	y *= f;
	z *= f;
	w *= f;
	return *this;
}

/*------------------------------------------------------------------------------
assignment operator
------------------------------------------------------------------------------*/
Vector4& Vector4::operator/=( float f ) {
	float fInv = 1.0f / f;
	x *= fInv;
	y *= fInv;
	z *= fInv;
	w *= fInv;
	return *this;
}

/*------------------------------------------------------------------------------
unary operator
------------------------------------------------------------------------------*/
Vector4 Vector4::operator+() const {
	return Vector4( *this );
}

/*------------------------------------------------------------------------------
unary operator
------------------------------------------------------------------------------*/
Vector4 Vector4::operator-() const {
	return Vector4( -x, -y, -z, -w );
}

/*------------------------------------------------------------------------------
binary operator
------------------------------------------------------------------------------*/
Vector4 Vector4::operator+( Vector4 const &v ) const {
	return Vector4( x + v.x, y + v.y, z + v.z, w + v.w );
}

/*------------------------------------------------------------------------------
binary operator
------------------------------------------------------------------------------*/
Vector4 Vector4::operator-( Vector4 const &v ) const {
	return Vector4( x - v.x, y - v.y, z - v.z, w - v.w );
}

/*------------------------------------------------------------------------------
binary operator (row vector * 4x4 matrix)
------------------------------------------------------------------------------*/
Vector4 Vector4::operator*( Matrix const &m ) const {
	return Vector4( x * m.m._00 + y * m.m._10 + z * m.m._20 + w * m.m._30,
				    x * m.m._01 + y * m.m._11 + z * m.m._21 + w * m.m._31,
				    x * m.m._02 + y * m.m._12 + z * m.m._22 + w * m.m._32,
					x * m.m._03 + y * m.m._13 + z * m.m._23 + w * m.m._33 );
}

/*------------------------------------------------------------------------------
binary operator
------------------------------------------------------------------------------*/
Vector4 Vector4::operator*( float f ) const {
	return Vector4( x * f, y * f, z * f, w * f );
}

/*------------------------------------------------------------------------------
binary operator
------------------------------------------------------------------------------*/
Vector4 Vector4::operator/( float f ) const {
	float fInv = 1.0f / f;
	return Vector4( x * fInv, y * fInv, z * fInv, w * fInv );
}

/*------------------------------------------------------------------------------
friend binary operator
------------------------------------------------------------------------------*/
Vector4 operator*( float f, Vector4 const &v ) {
	return Vector4( v.x * f, v.y * f, v.z * f, v.w * f );
}

/*------------------------------------------------------------------------------
comparison operator
------------------------------------------------------------------------------*/
bool Vector4::operator==( Vector4 const &v ) const {
	return x == v.x && y == v.y && z == v.z && w == v.w;
}

/*------------------------------------------------------------------------------
comparison operator
------------------------------------------------------------------------------*/
bool Vector4::operator!=( Vector4 const &v ) const {
	return x != v.x || y != v.y || z != v.z || w != v.w;
}

/*------------------------------------------------------------------------------
stream operator
------------------------------------------------------------------------------*/
std::ostream& operator<<( std::ostream &os, Vector4 const &v ) {
	os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
	return os;
}

/*------------------------------------------------------------------------------
negate all components
------------------------------------------------------------------------------*/
Vector4& Vector4::Negate() {
	x = -x;
	y = -y;
	z = -z;
	w = -w;
	return *this;
}

/*------------------------------------------------------------------------------
set all components to zero
------------------------------------------------------------------------------*/
Vector4& Vector4::Clear() {
	x = y = z = w = 0;
	return *this;
}

/*------------------------------------------------------------------------------
vector's magnitude
------------------------------------------------------------------------------*/
float Vector4::Length() const {
	return sqrtf( x * x + y * y + z * z + w * w );
}

/*------------------------------------------------------------------------------
vector's magnitude squared
------------------------------------------------------------------------------*/
float Vector4::LengthSquared() const {
	return x * x + y * y + z * z + w * w;
}

/*------------------------------------------------------------------------------
normalize the vector
------------------------------------------------------------------------------*/
Vector4& Vector4::Normalize() {
	if( x != 0 || y != 0 || z != 0 || w != 0 ) *this /= Length();
    else *this = Vector4( 0, 1, 0, 0 );
	return *this;
}

/*------------------------------------------------------------------------------
set this to the component-wise product
------------------------------------------------------------------------------*/
Vector4& Vector4::ComponentProductUpdate( Vector4 const &v ) {
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;
	return *this;
}

/*------------------------------------------------------------------------------
normalize a vector
------------------------------------------------------------------------------*/
Vector4 Normalize( Vector4 const &v ) {
	return Vector4( v ).Normalize();
}

/*------------------------------------------------------------------------------
dot product
------------------------------------------------------------------------------*/
float Dot( Vector4 const &a, Vector4 const &b ) {
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

/*------------------------------------------------------------------------------
component-wise product
------------------------------------------------------------------------------*/
Vector4 ComponentProduct( Vector4 const &a, Vector4 const &b ) {
	return Vector4( a.x * b.x,
				    a.y * b.y,
				    a.z * b.z,
					a.w * b.w );
}

/*------------------------------------------------------------------------------
length of a vector
------------------------------------------------------------------------------*/
float Length( Vector4 const &v ) {
	return v.Length();
}
 
/*------------------------------------------------------------------------------
length of a vector squared
------------------------------------------------------------------------------*/
float LengthSquared( Vector4 const &v ) {
	return v.LengthSquared();
}

/*------------------------------------------------------------------------------
transform a vector using a matrix
------------------------------------------------------------------------------*/
Vector4 Transform( Vector4 const &v, Matrix const &m ) {
	return v * m;
}