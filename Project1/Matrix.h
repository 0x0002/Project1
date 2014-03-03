#ifndef Matrix_H
#define Matrix_H

// forward declaration
#include <iosfwd>	// forward declaration of std::ostream
class Vector3;
class Vector4;
class Matrix33;
class Quaternion;

/*------------------------------------------------------------------------------
4x4 matrix
------------------------------------------------------------------------------*/
class Matrix {
public:
	// constructors
	Matrix() {}
	Matrix( Matrix const & );
	Matrix( float const * );
	Matrix( float _00, float _01, float _02, float _03,
		    float _10, float _11, float _12, float _13,
		    float _20, float _21, float _22, float _23,
		    float _30, float _31, float _32, float _33 );
    Matrix( Vector4 const &row0,
            Vector4 const &row1,
            Vector4 const &row2,
            Vector4 const &row3 );
    Matrix( Matrix33 const &rotation, Vector3 const &translation );

	// accessors
	float& operator()( uint32_t row, uint32_t col );
	float operator()( uint32_t row, uint32_t col ) const;
    float const* operator()( uint32_t row ) const;

	// casting operators
	operator float*();
	operator float const*() const;

	// assignment operators
	Matrix& operator*=( Matrix const & );
	Matrix& operator+=( Matrix const & );
	Matrix& operator-=( Matrix const & );
	Matrix& operator*=( float );
	Matrix& operator/=( float );

	// unary operators
	Matrix operator+() const;
	Matrix operator-() const;

    // binary operators
	Matrix operator*( Matrix const & ) const;
	Matrix operator+( Matrix const & ) const;
	Matrix operator-( Matrix const & ) const;
	Matrix operator*( float ) const;
	Matrix operator/( float ) const;

	friend Matrix operator*( float, Matrix const & );

	// comparison operators
	bool operator==( Matrix const & ) const;
	bool operator!=( Matrix const & ) const;

	// stream operator
	friend std::ostream& operator<<( std::ostream &, Matrix const & );

	// set this to the identity matrix
	Matrix& SetToIdentity();

	// set this to its transpose
	Matrix& Transpose();

	// set this to its inverse
	// returns the determinant
	// if the determinant is 0, the matrix is not invertible,
	// and is the matrix is not changed
	float Invert();

public:
	union {
		float data[ 4 ][ 4 ];	// matrix components as a 2D array

		struct {				// individual matrix components
			float _00, _01, _02, _03;
			float _10, _11, _12, _13;
			float _20, _21, _22, _23;
			float _30, _31, _32, _33;
		} m;
	};
};

// create an identity matrix
Matrix MatrixIdentity();

// transpose a matrix
Matrix MatrixTranspose( Matrix const & );

// component-wise absolute value
Matrix MatrixAbs( Matrix const & );

// invert a matrix
// returns the determinant if the argument is not NULL
// if the matrix is not invertible, the determinant will be zero,
// and the un-inverted matrix is returned
Matrix MatrixInverse( Matrix const &, float *determinant = NULL );

// create a translation matrix
Matrix MatrixTranslation( float tx, float ty, float tz );
Matrix MatrixTranslation( Vector3 const & );

// create a rotation matrix (angle is in radians)
Matrix MatrixRotationX( float angle );
Matrix MatrixRotationY( float angle );
Matrix MatrixRotationZ( float angle );
Matrix MatrixRotationYawPitchRoll( float yaw, float pitch, float roll );
Matrix MatrixRotationAxisAngle( Vector3 const &axis, float angle );
Matrix MatrixRotationQuaternion( Quaternion const & );

// create a scaling matrix
Matrix MatrixScaling( float s );
Matrix MatrixScaling( float sx, float sy, float sz );

// create a transform matrix
Matrix MatrixVQS( Vector3 const &v, Quaternion const &q, float s );

// create a left-handed view matrix
Matrix MatrixLookAtLH( Vector3 const &eye, Vector3 const &at, Vector3 const &up ); 

// create a left-handed projection matrix
Matrix MatrixOrthographicLH( float width, float height, float zNear, float zFar );
Matrix MatrixPerspectiveFovLH( float fov, float aspectRatio, float zNear, float zFar );

// create a right-handed view matrix
Matrix MatrixLookAtRH( Vector3 const &eye, Vector3 const &at, Vector3 const &up ); 

// create a right-handed projection matrix
Matrix MatrixOrthographicRH( float width, float height, float zNear, float zFar );
Matrix MatrixPerspectiveFovRH( float fov, float aspectRatio, float zNear, float zFar );

#endif // Matrix_H