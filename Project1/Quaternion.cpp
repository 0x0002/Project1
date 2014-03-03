#include "Precompiled.h"
#include "Quaternion.h"
#include <iostream>			// ostream
#include <cmath>			// square root
#include "Vector3.h"		// 3D vector

/*------------------------------------------------------------------------------
copy constructor
------------------------------------------------------------------------------*/
Quaternion::Quaternion( Quaternion const &q ) :
	x( q.x ), y( q.y ), z( q.z ), w( q.w ) {
}

/*------------------------------------------------------------------------------
conversion constructor
------------------------------------------------------------------------------*/
Quaternion::Quaternion( float const *f ) {
	*this = *(Quaternion const*)f;
}

/*------------------------------------------------------------------------------
constructor
------------------------------------------------------------------------------*/
Quaternion::Quaternion( float x, float y, float z, float w ) :
	x( x ), y( y ), z( z ), w( w ) {
}

/*------------------------------------------------------------------------------
casting operator
------------------------------------------------------------------------------*/
Quaternion::operator float*() {
	return (float*)this;
}

/*------------------------------------------------------------------------------
casting operator
------------------------------------------------------------------------------*/
Quaternion::operator float const*() const {
	return (float const*)this;
}

/*------------------------------------------------------------------------------
assignment operator
------------------------------------------------------------------------------*/
Quaternion& Quaternion::operator+=( Quaternion const &q ) {
	x += q.x;
	y += q.y;
	z += q.z;
	w += q.w;
	return *this;
}

/*------------------------------------------------------------------------------
assignment operator
------------------------------------------------------------------------------*/
Quaternion& Quaternion::operator-=( Quaternion const &q ) {
	x -= q.x;
	y -= q.y;
	z -= q.z;
	w -= q.w;
	return *this;
}

/*------------------------------------------------------------------------------
assignment operator
------------------------------------------------------------------------------*/
Quaternion& Quaternion::operator*=( Quaternion const &q ) {
	*this = Quaternion( q.w * x + q.x * w + q.y * z - q.z * y,
					    q.w * y + q.y * w + q.z * x - q.x * z,
					    q.w * z + q.z * w + q.x * y - q.y * x,
					    q.w * w - q.x * x - q.y * y - q.z * z);
	return *this;
}

/*------------------------------------------------------------------------------
assignment operator
------------------------------------------------------------------------------*/
Quaternion& Quaternion::operator*=( float f ) {
	x *= f;
	y *= f;
	z *= f;
	w *= f;
	return *this;
}

/*------------------------------------------------------------------------------
assignment operator
------------------------------------------------------------------------------*/
Quaternion& Quaternion::operator/=( float f ) {
	float invF = 1.0f / f;
	x *= invF;
	y *= invF;
	z *= invF;
	w *= invF;
	return *this;
}

/*------------------------------------------------------------------------------
unary operator
------------------------------------------------------------------------------*/
Quaternion Quaternion::operator+() const {
	return Quaternion( *this );
}

/*------------------------------------------------------------------------------
unary operator
------------------------------------------------------------------------------*/
Quaternion Quaternion::operator-() const {
	return Quaternion( -x, -y, -z, -w );
}

/*------------------------------------------------------------------------------
binary operator
------------------------------------------------------------------------------*/
Quaternion Quaternion::operator+( Quaternion const &q ) const {
	return Quaternion( x + q.x, y + q.y, z + q.z, w + q.w );
}

/*------------------------------------------------------------------------------
binary operator
------------------------------------------------------------------------------*/
Quaternion Quaternion::operator-( Quaternion const &q ) const {
	return Quaternion( x - q.x, y - q.y, z - q.z, w - q.w );
}

/*------------------------------------------------------------------------------
binary operator
------------------------------------------------------------------------------*/
Quaternion Quaternion::operator*( Quaternion const &q ) const {
	return Quaternion( q.w * x + q.x * w + q.y * z - q.z * y,
					   q.w * y + q.y * w + q.z * x - q.x * z,
					   q.w * z + q.z * w + q.x * y - q.y * x,
					   q.w * w - q.x * x - q.y * y - q.z * z);
}

/*------------------------------------------------------------------------------
binary operator
------------------------------------------------------------------------------*/
Quaternion Quaternion::operator*( float f ) const {
	return Quaternion( x * f, y * f, z * f, w * f );
}

/*------------------------------------------------------------------------------
binary operator
------------------------------------------------------------------------------*/
Quaternion Quaternion::operator/( float f ) const {
	float invF = 1.0f / f;
	return Quaternion( x * invF, y * invF, z * invF, w * invF );
}

/*------------------------------------------------------------------------------
friend binary operator
------------------------------------------------------------------------------*/
Quaternion operator*( float f, Quaternion const &q ) {
	return Quaternion( q.x * f, q.y * f, q.z * f, q.w * f );
}

/*------------------------------------------------------------------------------
comparison operator
------------------------------------------------------------------------------*/
bool Quaternion::operator==( Quaternion const &q ) const {
	return x == q.x && y == q.y && z == q.z && w == q.w;
}

/*------------------------------------------------------------------------------
comparison operator
------------------------------------------------------------------------------*/
bool Quaternion::operator!=( Quaternion const &q ) const {
	return x != q.x || y != q.y || z != q.z || w != q.w;
}

/*------------------------------------------------------------------------------
stream operator
------------------------------------------------------------------------------*/
std::ostream& operator<<( std::ostream &os, Quaternion const &q ) {
	os << "(" << q.x << ", " << q.y << ", " << q.z << ", " << q.w << ")";
	return os;
}

/*------------------------------------------------------------------------------
set this to an identity quaternion
------------------------------------------------------------------------------*/
Quaternion& Quaternion::Identity() {
	x = y = z = 0;
	w = 1;
	return *this;
}

/*------------------------------------------------------------------------------
quaternion's length 
------------------------------------------------------------------------------*/
float Quaternion::Length() const {
	return sqrt( x * x + y * y + z * z + w * w );
}

/*------------------------------------------------------------------------------
quaternion's length squared
------------------------------------------------------------------------------*/
float Quaternion::LengthSquared() const {
	return x * x + y * y + z * z + w * w;
}

/*------------------------------------------------------------------------------
normalize this quaternion
------------------------------------------------------------------------------*/
Quaternion& Quaternion::Normalize() {
	float lengthSquared = LengthSquared();
	float invLength = 0;
	if( lengthSquared ) {
		invLength = 1.0f / sqrtf( lengthSquared );
	}
	*this *= invLength;
	return *this;
}

/*------------------------------------------------------------------------------
set this quaternion to its conjugate
------------------------------------------------------------------------------*/
Quaternion& Quaternion::Conjugate() {
	x = -x;
	y = -y;
	z = -z;
	return *this;
}

/*------------------------------------------------------------------------------
set this quaternion to its inverse
------------------------------------------------------------------------------*/
Quaternion& Quaternion::Invert() {
	*this = Conjugate() / LengthSquared();
	return *this;
}

/*------------------------------------------------------------------------------
length of a quaternion
------------------------------------------------------------------------------*/
float Length( Quaternion const &q ) {
	return q.Length();
}

/*------------------------------------------------------------------------------
length of a quaternion squared
------------------------------------------------------------------------------*/
float LengthSquared( Quaternion const &q ) {
	return q.LengthSquared();
}

/*------------------------------------------------------------------------------
normalize a quaternion
------------------------------------------------------------------------------*/
Quaternion Normalize( Quaternion const &q ) {
	return Quaternion( q ).Normalize();
}

/*------------------------------------------------------------------------------
quaternion conjugate
------------------------------------------------------------------------------*/
Quaternion Conjugate( Quaternion const &q ) {
	return Quaternion( -q.x, -q.y, -q.z, q.w );
}

/*------------------------------------------------------------------------------
quaternion inverse
------------------------------------------------------------------------------*/
Quaternion Inverse( Quaternion const &q ) {
	return Conjugate( q ) / q.LengthSquared();
}

/*------------------------------------------------------------------------------
spherical linear interpolation. t should be [0, 1]
------------------------------------------------------------------------------*/
Quaternion Slerp( Quaternion const &a, Quaternion const &b, float t ) {
	float cosOmega = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; 

	bool flip = cosOmega < 0;
	if( flip ) cosOmega = -cosOmega;

    float alpha;
    float beta;
	if( ( 1.0f - cosOmega ) > kEpsilon ) {
        // slerp
		float omega       = acosf( cosOmega );
		float invSinOmega = 1.0f / sinf( omega );
		alpha = sinf( ( 1.0f - t ) * omega ) * invSinOmega;
		beta  = sinf( t * omega ) * invSinOmega;
	}
	else {
        // lerp
		alpha = ( 1.0f - t );
		beta  = t;
	}

	if( flip ) beta = -beta;
    return alpha * a + beta * b;
}

/*------------------------------------------------------------------------------
create a quaterion that represents rotation about an axis (angle is radians)
the resulting quaternion is normalized
------------------------------------------------------------------------------*/
Quaternion QuaternionRotationAxisAngle( Vector3 const &axis, float angle ) {
	Vector3 a = Normalize( axis) ;
	float halfAngle = angle * 0.5f;
	float s = sinf( halfAngle );
	return Quaternion( s * a.x, s * a.y, s * a.z, cosf( halfAngle ) );
}

/*------------------------------------------------------------------------------
create a quaternion that represents a yaw-pitch-roll rotation
the resulting quaternion is normalized
------------------------------------------------------------------------------*/
Quaternion QuaternionRotationYawPitchRoll( float yaw, float pitch, float roll ) {
	float halfYaw = yaw * 0.5f;
	float sy = sinf( halfYaw );
	float cy = cosf( halfYaw );

	float halfPitch = pitch * 0.5f;
	float sp = sinf( halfPitch );
	float cp = cosf( halfPitch );

	float halfRoll = roll * 0.5f;
	float sr = sinf( halfRoll );
	float cr = cosf( halfRoll );

	float cycp = cy * cp;
	float sysp = sy * sp;

	return Quaternion( cy * sp * cr + sy * cp * sr,
					   sy * cp * cr - cy * sp * sr,
					   cycp * sr -  sysp * cr,
					   cycp * cr +  sysp * sr);
}

/*------------------------------------------------------------------------------
convert from rotation matrix to quaternion
------------------------------------------------------------------------------*/
Quaternion QuaternionRotationMatrix( Matrix const &m ) {
    Quaternion q;

    float trace = m.m._00 + m.m._11 + m.m._22;
    if( trace > 0.0f ) {
        float s = 0.5f / sqrtf( trace + 1.0f );
        q.w = 0.25f / s;
        q.x = ( m.m._21 - m.m._12 ) * s;
        q.y = ( m.m._02 - m.m._20 ) * s;
        q.z = ( m.m._10 - m.m._01 ) * s;
    }
    else if ( m.m._00 > m.m._11 && m.m._00 > m.m._22 ) {
        float s = 2.0f * sqrtf( 1.0f + m.m._00 - m.m._11 - m.m._22 );
        q.w = ( m.m._21 - m.m._12 ) / s;
        q.x = 0.25f * s;
        q.y = ( m.m._01 + m.m._10 ) / s;
        q.z = ( m.m._02 + m.m._20 ) / s;
    }
    else if ( m.m._11 > m.m._22 ) {
        float s = 2.0f * sqrtf( 1.0f + m.m._11 - m.m._00 - m.m._22 );
        q.w = ( m.m._02 - m.m._20 ) / s;
        q.x = ( m.m._01 + m.m._10 ) / s;
        q.y = 0.25f * s;
        q.z = ( m.m._12 + m.m._21 ) / s;
    }
    else {
        float s = 2.0f * sqrtf( 1.0f + m.m._22 - m.m._00 - m.m._11 );
        q.w = ( m.m._10 - m.m._01 ) / s;
        q.x = ( m.m._02 + m.m._20 ) / s;
        q.y = ( m.m._12 + m.m._21 ) / s;
        q.z = 0.25f * s;
    }
    return q;
}