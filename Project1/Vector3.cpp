#include "Precompiled.h"
#include "Vector3.h"
#include <iostream>			// ostream
#include <cmath>			// square root
#include "Vector2.h"		// 2d vector
#include "Vector4.h"		// 3d vector
#include "Matrix.h"			// 4x4 matrix
#include "Quaternion.h"		// Quaternion

/*------------------------------------------------------------------------------
conversion constructor
------------------------------------------------------------------------------*/
Vector3::Vector3( Vector2 const &v, float z ) :
	x( v.x ), y( v.y ), z( z ) {
}

/*------------------------------------------------------------------------------
copy constructor
------------------------------------------------------------------------------*/
Vector3::Vector3( Vector3 const &v ) :
	x( v.x ), y( v.y ), z( v.z ) {
}

/*------------------------------------------------------------------------------
conversion constructor. copies xyz from the Vector4 into a Vector3
------------------------------------------------------------------------------*/
Vector3::Vector3( Vector4 const &v ) :
	x( v.x ), y( v.y ), z( v.z ) {
}

/*------------------------------------------------------------------------------
conversion constructor
------------------------------------------------------------------------------*/
Vector3::Vector3( float const *f  ) {
	*this = *(Vector3 const*)f;
}

/*------------------------------------------------------------------------------
constructor
------------------------------------------------------------------------------*/
Vector3::Vector3( float x, float y, float z ) :
	x( x ), y( y ), z( z ) {
}

/*------------------------------------------------------------------------------
casting operator
------------------------------------------------------------------------------*/
Vector3::operator float*() {
	return (float*)this;
}

/*------------------------------------------------------------------------------
casting operator
------------------------------------------------------------------------------*/
Vector3::operator float const*() const {
	return (float const*)this;
}

/*------------------------------------------------------------------------------
assignment operator
------------------------------------------------------------------------------*/
Vector3& Vector3::operator+=( Vector3 const &v ) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

/*------------------------------------------------------------------------------
assignment operator
------------------------------------------------------------------------------*/
Vector3& Vector3::operator-=( Vector3 const &v ) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

/*------------------------------------------------------------------------------
assignment operator (cross product)
------------------------------------------------------------------------------*/
Vector3& Vector3::operator%=( Vector3 const &v ) {
	*this = Vector3( y * v.z - z * v.y,
				     z * v.x - x * v.z,
				     x * v.y - y * v.x );
	return *this;
}

/*------------------------------------------------------------------------------
assignment operator ((x,y,z,1) row vector * 4x4 matrix)
------------------------------------------------------------------------------*/
Vector3& Vector3::operator*=( Matrix const &m) {
	*this = *this * m;
	return *this;
}

/*------------------------------------------------------------------------------
assignment operator
------------------------------------------------------------------------------*/
Vector3& Vector3::operator*=( Matrix33 const &m) {
	*this = *this * m;
	return *this;
}


/*------------------------------------------------------------------------------
assignment operator
------------------------------------------------------------------------------*/
Vector3& Vector3::operator*=( float f ) {
	x *= f;
	y *= f;
	z *= f;
	return *this;
}

/*------------------------------------------------------------------------------
assignment operator
------------------------------------------------------------------------------*/
Vector3& Vector3::operator/=( float f ) {
	float fInv = 1.0f / f;
	x *= fInv;
	y *= fInv;
	z *= fInv;
	return *this;
}

/*------------------------------------------------------------------------------
unary operator
------------------------------------------------------------------------------*/
Vector3 Vector3::operator+() const {
	return Vector3( *this );
}

/*------------------------------------------------------------------------------
unary operator
------------------------------------------------------------------------------*/
Vector3 Vector3::operator-() const {
	return Vector3( -x, -y, -z );
}

/*------------------------------------------------------------------------------
binary operator
------------------------------------------------------------------------------*/
Vector3 Vector3::operator+( Vector3 const &v ) const {
	return Vector3( x + v.x, y + v.y, z + v.z );
}

/*------------------------------------------------------------------------------
binary operator
------------------------------------------------------------------------------*/
Vector3 Vector3::operator-( Vector3 const &v ) const {
	return Vector3( x - v.x, y - v.y, z - v.z );
}

/*------------------------------------------------------------------------------
binary operator (cross product)
------------------------------------------------------------------------------*/
Vector3 Vector3::operator%( Vector3 const &v ) const {
	return Vector3( y * v.z - z * v.y,
				    z * v.x - x * v.z,
				    x * v.y - y * v.x );
}

/*------------------------------------------------------------------------------
binary operator ((x,y,z,1) row vector * 4x4 matrix) and projects back to w = 1
------------------------------------------------------------------------------*/
Vector3 Vector3::operator*( Matrix const &m ) const {
	float w = x * m.m._03 + y * m.m._13 + z * m.m._23 + m.m._33;

	// guard against divide by zero
	float invW = 0;
	if( w ) invW = 1.0f / w;

	return Vector3( x * m.m._00 + y * m.m._10 + z * m.m._20 + m.m._30,
				    x * m.m._01 + y * m.m._11 + z * m.m._21 + m.m._31,
				    x * m.m._02 + y * m.m._12 + z * m.m._22 + m.m._32 ) * invW;
}

/*------------------------------------------------------------------------------
binary operator
------------------------------------------------------------------------------*/
Vector3 Vector3::operator*( Matrix33 const &m ) const {
	return Vector3( x * m.m._00 + y * m.m._10 + z * m.m._20,
				    x * m.m._01 + y * m.m._11 + z * m.m._21,
				    x * m.m._02 + y * m.m._12 + z * m.m._22 );
}

/*------------------------------------------------------------------------------
binary operator
------------------------------------------------------------------------------*/
Vector3 Vector3::operator*( float f ) const {
	return Vector3( x * f, y * f, z * f );
}

/*------------------------------------------------------------------------------
binary operator
------------------------------------------------------------------------------*/
Vector3 Vector3::operator/( float f ) const {
	float fInv = 1.0f / f;
	return Vector3( x * fInv, y * fInv, z * fInv );
}

/*------------------------------------------------------------------------------
friend binary operator
------------------------------------------------------------------------------*/
Vector3 operator*( float f, Vector3 const &v ) {
	return Vector3( v.x * f, v.y * f, v.z * f );
}

/*------------------------------------------------------------------------------
comparison operator
------------------------------------------------------------------------------*/
bool Vector3::operator==( Vector3 const &v ) const {
	return x == v.x && y == v.y && z == v.z;
}

/*------------------------------------------------------------------------------
comparison operator
------------------------------------------------------------------------------*/
bool Vector3::operator!=( Vector3 const &v ) const {
	return x != v.x || y != v.y || z != v.z;
}

/*------------------------------------------------------------------------------
stream operator
------------------------------------------------------------------------------*/
std::ostream& operator<<( std::ostream &os, Vector3 const &v ) {
	os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return os;
}

/*------------------------------------------------------------------------------
negate all components
------------------------------------------------------------------------------*/
Vector3& Vector3::Negate() {
	x = -x;
	y = -y;
	z = -z;
	return *this;
}

/*------------------------------------------------------------------------------
set all components to zero
------------------------------------------------------------------------------*/
Vector3& Vector3::Clear() {
	x = y = z = 0;
	return *this;
}

/*------------------------------------------------------------------------------
vector's magnitude
------------------------------------------------------------------------------*/
float Vector3::Length() const {
	return sqrtf( x * x + y * y + z * z );
}

/*------------------------------------------------------------------------------
vector's magnitude squared
------------------------------------------------------------------------------*/
float Vector3::LengthSquared() const {
	return x * x + y * y + z * z;
}

/*------------------------------------------------------------------------------
normalize the vector
------------------------------------------------------------------------------*/
Vector3& Vector3::Normalize() {
	if( x != 0 || y != 0 || z != 0 ) *this /= Length();
    else *this = Vector3( 0, 1, 0 );
	return *this;
}

/*------------------------------------------------------------------------------
set this to the component-wise product
------------------------------------------------------------------------------*/
Vector3& Vector3::ComponentProductUpdate( Vector3 const &v ) {
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

/*------------------------------------------------------------------------------
normalize a vector
------------------------------------------------------------------------------*/
Vector3 Normalize( Vector3 const &v ) {
	return Vector3( v ).Normalize();
}

/*------------------------------------------------------------------------------
dot product
------------------------------------------------------------------------------*/
float Dot( Vector3 const &a, Vector3 const &b ) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

/*------------------------------------------------------------------------------
vector product
------------------------------------------------------------------------------*/
Vector3 Cross( Vector3 const &a, Vector3 const &b ) {
	return a % b;
}

/*------------------------------------------------------------------------------
component-wise product
------------------------------------------------------------------------------*/
Vector3 ComponentProduct( Vector3 const &a, Vector3 const &b ) {
	return Vector3( a.x * b.x,
				    a.y * b.y,
				    a.z * b.z );
}

/*------------------------------------------------------------------------------
length of a vector
------------------------------------------------------------------------------*/
float Length( Vector3 const &v ) {
	return v.Length();
}
 
/*------------------------------------------------------------------------------
length of a vector squared
------------------------------------------------------------------------------*/
float LengthSquared( Vector3 const &v ) {
	return v.LengthSquared();
}

/*------------------------------------------------------------------------------
transform a vector using a matrix
------------------------------------------------------------------------------*/
Vector3 Transform( Vector3 const &v, Matrix const &m ) {
	return v * m;
}

/*------------------------------------------------------------------------------
transform a vector using a matrix
------------------------------------------------------------------------------*/
Vector3 Transform( Vector3 const &v, Matrix33 const &m ) {
	return v * m;
}

/*------------------------------------------------------------------------------
transform a vector usinga  quaternion
the quaternion should be normalized
------------------------------------------------------------------------------*/
Vector3 Transform( Vector3 const &v, Quaternion const &q ) {
	// create a pure quaternion from the vector
	Quaternion pure = Quaternion( v.x, v.y, v.z, 0 );
	
	// because the quaternion passed in is normalized,
	// the inverse is equal to the conujgate

	// Conjugate(q) and q are swapped from standard notation because this is a row vector I think
	// Quaternion rotations will match Matrix rotations
	pure = Conjugate( q ) * pure * q;
	
	return Vector3( pure.x, pure.y, pure.z );
}

/*------------------------------------------------------------------------------
linear interpolation. t should be [0, 1]
------------------------------------------------------------------------------*/
Vector3 Lerp( Vector3 const &a, Vector3 const &b, float t ) {
    return ( 1.0f - t ) * a + t * b;
}