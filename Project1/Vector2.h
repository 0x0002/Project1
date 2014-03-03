#ifndef VECTOR2_H
#define VECTOR2_H

// forward declarations
#include <iosfwd>	// forward declaration of std::ostream
class Vector3;
class Vector4;
class Matrix22;

/*------------------------------------------------------------------------------
2d vector
------------------------------------------------------------------------------*/
class Vector2{
public:
	// constructors
	Vector2() {}
	Vector2( Vector2 const & );
	explicit Vector2( Vector3 const & ); // copies xy from the Vector3 into a Vector2
	explicit Vector2( Vector4 const & ); // copies xy from the Vector4 into a Vector2
	Vector2( float const * );
	Vector2( float x, float y );

	// casting operators
	operator float*();
	operator float const*() const;

	// assignment operators
	Vector2& operator+=( Vector2 const & );
	Vector2& operator-=( Vector2 const & );
    Vector2& operator*=( Matrix22 const & );
	Vector2& operator*=( float );
	Vector2& operator/=( float );

	// unary operators
	Vector2 operator+() const;
	Vector2 operator-() const;

	// binary operators
	Vector2 operator+( Vector2 const & ) const;
	Vector2 operator-( Vector2 const & ) const;
    Vector2 operator*( Matrix22 const & ) const;
	Vector2 operator*( float ) const;
	Vector2 operator/( float ) const;

	friend Vector2 operator*( float, Vector2 const & );

	// comparison operators
	bool operator==( Vector2 const & ) const;
	bool operator!=( Vector2 const & ) const;

	// stream operator
	friend std::ostream& operator<<( std::ostream &, Vector2 const & );

	// negate all components
	Vector2& Negate();

	// set all components to zero
	Vector2& Clear();

	// vector's magnitude
	float Length() const;

	// vector's magnitude squared
	float LengthSquared() const;

	// normalize the vector
	Vector2& Normalize();

	// set this to the component-wise product
	Vector2& ComponentProductUpdate( Vector2 const & );
    
    // returns the angle representation in radian
    float ToAngle() const;

    // returns the angle representation in radian, opposite hand
    float ToAngle2() const;

public:
	float x, y;		// vector components
};

// normalize a vector
Vector2 Normalize( Vector2 const & );

// dot product
float Dot( Vector2 const &, Vector2 const & );

// component-wise product
Vector2 ComponentProduct( Vector2 const &, Vector2 const & );

// length of a vector
float Length( Vector2 const & );

// length of a vector squared
float LengthSquared( Vector2 const & );

// find an orthogonal vector
Vector2 Orthogonal( Vector2 const & );

#endif // VECTOR2_H