#ifndef VECTOR4_H
#define VECTOR4_H

// forward declarations
#include <iosfwd>	// forward declaration of std::ostream
class Vector2;
class Vector3;
class Matrix;

/*------------------------------------------------------------------------------
4d vector
------------------------------------------------------------------------------*/
class Vector4{
public:
	// constructors
	Vector4() {}
	Vector4( Vector2 const &, float z, float w );
	Vector4( Vector3 const &, float w);
	Vector4( Vector4 const & );
	Vector4( float const * );
	Vector4( float x, float y, float z, float w );

	// casting operators
	operator float*();
	operator float const*() const;

	// assignment operators
	Vector4& operator+=( Vector4 const & );
	Vector4& operator-=( Vector4 const & );
	Vector4& operator*=( Matrix const & );
	Vector4& operator*=( float );
	Vector4& operator/=( float );

	// unary operators
	Vector4 operator+() const;
	Vector4 operator-() const;

	// binary operators
	Vector4 operator+( Vector4 const & ) const;
	Vector4 operator-( Vector4 const & ) const;
	Vector4 operator*( Matrix const & ) const;
	Vector4 operator*( float ) const;
	Vector4 operator/( float ) const;

	friend Vector4 operator*( float, Vector4 const & );

	// comparison operators
	bool operator==( Vector4 const & ) const;
	bool operator!=( Vector4 const & ) const;

	// stream operator
	friend std::ostream& operator<<( std::ostream &, Vector4 const & );

	// negate all components
	Vector4& Negate();

	// set all components to zero
	Vector4& Clear();

	// vector's magnitude
	float Length() const;

	// vector's magnitude squared
	float LengthSquared() const;

	// normalize the vector
	Vector4& Normalize();

	// set this to the component-wise product
	Vector4& ComponentProductUpdate( Vector4 const & );

public:
	float x, y, z, w;		// vector components
};

// normalize a vector
Vector4 Normalize( Vector4 const & );

// dot product
float Dot( Vector4 const &, Vector4 const & );

// component-wise product
Vector4 ComponentProduct( Vector4 const &, Vector4 const & );

// length of a vector
float Length( Vector4 const & );

// length of a vector squared
float LengthSquared( Vector4 const & );

// transform a vector using a matrix
Vector4 Transform( Vector4 const &, Matrix const & );

#endif // VECTOR4_H