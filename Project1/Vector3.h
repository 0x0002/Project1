#ifndef VECTOR3_H
#define VECTOR3_H

// forward declarations
#include <iosfwd>	// forward declaration of std::ostream
class Vector2;
class Vector4;
class Matrix;
class Matrix33;
class Quaternion;

/*------------------------------------------------------------------------------
3d vector
------------------------------------------------------------------------------*/
class Vector3{
public:
	// constructors
	Vector3() {}
	Vector3( Vector2 const &, float z );
	Vector3( Vector3 const & );
	explicit Vector3( Vector4 const & );		// copies xyz from the Vector4 into a Vector3
	Vector3( float const * );
	Vector3( float x, float y, float z );

	// casting operators
	operator float*();
	operator float const*() const;

	// assignment operators
	Vector3& operator+=( Vector3 const & );
	Vector3& operator-=( Vector3 const & );
	Vector3& operator%=( Vector3 const & );		// cross product
	Vector3& operator*=( Matrix const & );
    Vector3& operator*=( Matrix33 const & );
	Vector3& operator*=( float );
	Vector3& operator/=( float );

	// unary operators
	Vector3 operator+() const;
	Vector3 operator-() const;

	// binary operators
	Vector3 operator+( Vector3 const & ) const;
	Vector3 operator-( Vector3 const & ) const;
	Vector3 operator%( Vector3 const & ) const;	// cross product
	Vector3 operator*( Matrix const & ) const;
    Vector3 operator*( Matrix33 const & ) const;
	Vector3 operator*( float ) const;
	Vector3 operator/( float ) const;

	friend Vector3 operator*( float, Vector3 const & );

	// comparison operators
	bool operator==( Vector3 const & ) const;
	bool operator!=( Vector3 const & ) const;

	// stream operator
	friend std::ostream& operator<<( std::ostream &, Vector3 const & );

	// negate all components
	Vector3& Negate();

	// set all components to zero
	Vector3& Clear();

	// vector's magnitude
	float Length() const;

	// vector's magnitude squared
	float LengthSquared() const;

	// normalize the vector
	Vector3& Normalize();

	// set this to the component-wise product
	Vector3& ComponentProductUpdate( Vector3 const & );

public:
	float x, y, z;		// vector components
};

// normalize a vector
Vector3 Normalize( Vector3 const & );

// dot product
float Dot( Vector3 const &, Vector3 const & );

// vector product
Vector3 Cross( Vector3 const &a, Vector3 const &b );

// component-wise product
Vector3 ComponentProduct( Vector3 const &, Vector3 const & );

// length of a vector
float Length( Vector3 const & );

// length of a vector squared
float LengthSquared( Vector3 const & );

// transform a vector using a matrix
Vector3 Transform( Vector3 const &, Matrix const & );
Vector3 Transform( Vector3 const &, Matrix33 const & );

// transform a vector using a quaternion
// the quaternion should be normalized
Vector3 Transform( Vector3 const &, Quaternion const & );

// linear interpolation. t should be [0, 1]
Vector3 Lerp( Vector3 const &a, Vector3 const &b, float t );

#endif // VECTOR3_H