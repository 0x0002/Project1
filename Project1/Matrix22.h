#ifndef MATRIX22_H
#define MATRIX22_H

// forward declaration
#include <iosfwd>	    // forward declaration of std::ostream
class Vector2;

/*------------------------------------------------------------------------------
2x2 matrix
------------------------------------------------------------------------------*/
class Matrix22 {
public:
	// constructors
	Matrix22() {}
	Matrix22( Matrix22 const & );
	Matrix22( float const * );
	Matrix22( float _00, float _01,
		      float _10, float _11 );
    Matrix22( Vector2 const &row0, Vector2 const &row1 );

	// accessors
	float&   operator()( uint32_t row, uint32_t col );
	float    operator()( uint32_t row, uint32_t col ) const;
	float const* operator()( uint32_t row ) const;

	// casting operators
	operator float*();
	operator float const*() const;

	// assignment operators
	Matrix22& operator*=( Matrix22 const & );
	Matrix22& operator+=( Matrix22 const & );
	Matrix22& operator-=( Matrix22 const & );
	Matrix22& operator*=( float );
	Matrix22& operator/=( float );

	// unary operators
	Matrix22 operator+() const;
	Matrix22 operator-() const;

    // binary operators
	Matrix22 operator*( Matrix22 const & ) const;
	Matrix22 operator+( Matrix22 const & ) const;
	Matrix22 operator-( Matrix22 const & ) const;
	Matrix22 operator*( float ) const;
	Matrix22 operator/( float ) const;

	friend Matrix22 operator*( float, Matrix22 const & );

	// comparison operators
	bool operator==( Matrix22 const & ) const;
	bool operator!=( Matrix22 const & ) const;

	// stream operator
	friend std::ostream& operator<<( std::ostream &, Matrix22 const & );

	// set this to the identity matrix
	Matrix22& SetToIdentity();

	// set this to its transpose
	Matrix22& Transpose();

public:
	union{
		float data[ 2 ][ 2 ];	// matrix components as a 2D array

		struct Components {		// individual matrix components
			float _00, _01;
			float _10, _11;
		} m;
	};
};

// create an identity matrix
Matrix22 Matrix22Identity();

// transpose a matrix
Matrix22 Matrix22Transpose( Matrix22 const & );

// component-wise absolute value
Matrix22 Matrix22Abs( Matrix22 const & );

// create a rotation matrix (angle is in radians)
Matrix22 Matrix22Rotation( float angle );

// create a scaling matrix
Matrix22 Matrix22Scaling( float s );
Matrix22 Matrix22Scaling( float sx, float sy );

#endif // MATRIX22_H