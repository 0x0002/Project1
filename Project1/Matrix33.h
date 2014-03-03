#ifndef MATRIX33_H
#define MATRIX33_H

// forward declaration
#include <iosfwd>	    // forward declaration of std::ostream
class Vector3;
class Matrix;
class Quaternion;

/*------------------------------------------------------------------------------
3x3 matrix
------------------------------------------------------------------------------*/
class Matrix33 {
public:
	// constructors
	Matrix33() {}
	Matrix33( Matrix33 const & );
	Matrix33( float const * );
	Matrix33( float _00, float _01, float _02,
		      float _10, float _11, float _12,
		      float _20, float _21, float _22 );
    Matrix33( Vector3 const &row0, Vector3 const &row1, Vector3 const &row2 );
    Matrix33( Matrix const &m );

	// accessors
	float&   operator()( uint32_t row, uint32_t col );
	float    operator()( uint32_t row, uint32_t col ) const;
	float const* operator()( uint32_t row ) const;

	// casting operators
	operator float*();
	operator float const*() const;

	// assignment operators
	Matrix33& operator*=( Matrix33 const & );
	Matrix33& operator+=( Matrix33 const & );
	Matrix33& operator-=( Matrix33 const & );
	Matrix33& operator*=( float );
	Matrix33& operator/=( float );

	// unary operators
	Matrix33 operator+() const;
	Matrix33 operator-() const;

    // binary operators
	Matrix33 operator*( Matrix33 const & ) const;
	Matrix33 operator+( Matrix33 const & ) const;
	Matrix33 operator-( Matrix33 const & ) const;
	Matrix33 operator*( float ) const;
	Matrix33 operator/( float ) const;

	friend Matrix33 operator*( float, Matrix33 const & );

	// comparison operators
	bool operator==( Matrix33 const & ) const;
	bool operator!=( Matrix33 const & ) const;

	// stream operator
	friend std::ostream& operator<<( std::ostream &, Matrix33 const & );

	// set this to the identity matrix
	Matrix33& SetToIdentity();

	// set this to its transpose
	Matrix33& Transpose();

public:
	union{
		float data[ 3 ][ 3 ];	// matrix components as a 2D array

		struct Components {		// individual matrix components
			float _00, _01, _02;
			float _10, _11, _12;
			float _20, _21, _22;
		} m;
	};
};

// create an identity matrix
Matrix33 Matrix33Identity();

// transpose a matrix
Matrix33 Matrix33Transpose( Matrix33 const & );

// component-wise absolute value
Matrix33 Matrix33Abs( Matrix33 const & );

// create a rotation matrix (angle is in radians)
Matrix33 Matrix33RotationX( float angle );
Matrix33 Matrix33RotationY( float angle );
Matrix33 Matrix33RotationZ( float angle );
Matrix33 Matrix33RotationYawPitchRoll( float yaw, float pitch, float roll );
Matrix33 Matrix33RotationAxisAngle( Vector3 const &axis, float angle );
Matrix33 Matrix33RotationQuaternion( Quaternion const & );

// create a scaling matrix
Matrix33 Matrix33Scaling( float s );
Matrix33 Matrix33Scaling( float sx, float sy, float sz );

#endif // MATRIX33_H