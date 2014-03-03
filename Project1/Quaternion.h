#ifndef QUATERNION_H
#define QUATERNION_H

// forward declarations
#include <iosfwd>	// forward declaration of std::ostream
class Vector3;
class Matrix;

/*------------------------------------------------------------------------------
quaternion
------------------------------------------------------------------------------*/
class Quaternion{
public:
	// constructors
	Quaternion() {}
	Quaternion( Quaternion const & );
	Quaternion( float const * );
	Quaternion( float x, float y, float z, float w );

	// casting operators
	operator float*();
	operator float const*() const;

	// assignment operators
	Quaternion& operator+=( Quaternion const & );
	Quaternion& operator-=( Quaternion const & );
	Quaternion& operator*=( Quaternion const & );
	Quaternion& operator*=( float );
	Quaternion& operator/=( float );

	// unary operators
	Quaternion operator+() const;
	Quaternion operator-() const;

	// binary operators
	Quaternion operator+( Quaternion const & ) const;
	Quaternion operator-( Quaternion const & ) const;
	Quaternion operator*( Quaternion const & ) const;
	Quaternion operator*( float ) const;
	Quaternion operator/( float ) const;

	friend Quaternion operator*( float, Quaternion const & );

	// comparison operators
	bool operator==( Quaternion const & ) const;
	bool operator!=( Quaternion const & ) const;

	// stream operator
	friend std::ostream& operator<<( std::ostream &, Quaternion const & );

	// set this to an identity quaternion
	Quaternion& Identity();

	// quaternion's length 
	float Length() const;

	// quaternion's length squared
	float LengthSquared() const;

	// normalize this quaternion
	Quaternion& Normalize();

	// set this quaternion to its conjugate
	Quaternion& Conjugate();

	// set this quaternion to its inverse
	Quaternion& Invert();

public:
	float x, y, z;		// vector part
	float w;			// scalar part
};

// length of a quaternion
float Length( Quaternion const & );

// length of a quaternion squared
float LengthSquared( Quaternion const & );

// normalize a quaternion
Quaternion Normalize( Quaternion const & );

// quaternion conjugate
Quaternion Conjugate( Quaternion const & );

// quaternion inverse
Quaternion Inverse( Quaternion const & );

// spherical linear interpolation. t should be [0, 1]
Quaternion Slerp( Quaternion const &a, Quaternion const &b, float t );

// create a quaterion that represents rotation about an axis (angle is radians)
// the resulting quaternion is normalized
Quaternion QuaternionRotationAxisAngle( Vector3 const &axis, float angle );

// create a quaternion that represents a yaw-pitch-roll rotation
// the resulting quaternion is normalized
Quaternion QuaternionRotationYawPitchRoll( float yaw, float pitch, float roll );

// convert from rotation matrix to quaternion
Quaternion QuaternionRotationMatrix( Matrix const &m );

#endif // QUATERNION_H