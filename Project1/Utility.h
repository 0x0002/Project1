#ifndef UTILITY_H
#define UTILITY_H

#include "Quaternion.h"         // quaternions
#include "Matrix.h"             // 4x4 matrix
#include "Vector4.h"            // 4d vector
#include "Vector3.h"            // 3d vector
#include "Vector2.h"            // 2d vector

extern const uint32_t crcLookup[ 256 ];

/*------------------------------------------------------------------------------
return the smaller of two values
------------------------------------------------------------------------------*/
inline float Min( float a, float b )                    { return ( a < b ) ? a : b; }

inline double Min( double a, double b )                 { return ( a < b ) ? a : b; }

inline int8_t Min( int8_t a, int8_t b )                 { return ( a < b ) ? a : b; }

inline int8_t Min( uint8_t a, uint8_t b )               { return ( a < b ) ? a : b; }

inline int16_t Min( int16_t a, int16_t b )              { return ( a < b ) ? a : b; }

inline int16_t Min( uint16_t a, uint16_t b )            { return ( a < b ) ? a : b; }

inline int32_t Min( int32_t a, int32_t b )              { return ( a < b ) ? a : b; }

inline uint32_t Min( uint32_t a, uint32_t b )           { return ( a < b ) ? a : b; }

inline int64_t Min( int64_t a, int64_t b )              { return ( a < b ) ? a : b; }

inline uint64_t Min( uint64_t a, uint64_t b )           { return ( a < b ) ? a : b; }

inline void* Min( void *a, void *b )                    { return ( a < b ) ? a : b; }

inline void const* Min( void const *a, void const *b )  { return ( a < b ) ? a : b; }

/*------------------------------------------------------------------------------
return the larger of two values
------------------------------------------------------------------------------*/
inline float Max( float a, float b )                    { return ( a > b ) ? a : b; }

inline double Max( double a, double b )                 { return ( a > b ) ? a : b; }

inline int8_t Max( int8_t a, int8_t b )                 { return ( a > b ) ? a : b; }

inline int8_t Max( uint8_t a, uint8_t b )               { return ( a > b ) ? a : b; }

inline int16_t Max( int16_t a, int16_t b )              { return ( a > b ) ? a : b; }

inline int16_t Max( uint16_t a, uint16_t b )            { return ( a > b ) ? a : b; }

inline int32_t Max( int32_t a, int32_t b )              { return ( a > b ) ? a : b; }

inline uint32_t Max( uint32_t a, uint32_t b )           { return ( a > b ) ? a : b; }

inline int64_t Max( int64_t a, int64_t b )              { return ( a > b ) ? a : b; }

inline uint64_t Max( uint64_t a, uint64_t b )           { return ( a > b ) ? a : b; }

inline void* Max( void *a, void *b )                    { return ( a > b ) ? a : b; }

inline void const* Max( void const *a, void const *b )  { return ( a > b ) ? a : b; }

/*------------------------------------------------------------------------------
dot product of float arrays
------------------------------------------------------------------------------*/
inline float Dot( float const *a, float const *b, uint32_t length ) {
    float sum = 0;
    float const *currentA = a;
    float const *currentB = b;
    for( uint32_t i = 0; i < length; ++i, ++currentA, ++currentB ) sum += *currentA * *currentB;
    return sum;
}

/*------------------------------------------------------------------------------
float == float comparison within an epsilon

absolute tolerance test
if(fabs(x-y) <= absTol) ...
fails when a and b are large

relative tolerance test
if(fabs(x-y) <= relTol * max(fabs(x), fabs(y)) ...
fails when a and be are small

need to use both absolute and relative comparisons

see http://realtimecollisiondetection.net/blog/?p=89 for more info
------------------------------------------------------------------------------*/
inline bool AlmostEqual( float a,
                        float b,
                        float absTol = 0.000001f ,
                        float relTol = 0.01f ) {
                            // absolute and relative comparisons are combined into a single comparison
                            return fabs( a - b ) <= Max( absTol, relTol * Max( fabs( a ), fabs( b ) ) );
}

/*------------------------------------------------------------------------------
clamp to the specified range [min, max]
------------------------------------------------------------------------------*/
inline void Clamp( int16_t *val, int16_t min, int16_t max ) { *val = Min( Max( *val, min ), max ); }

inline void Clamp( float *val, float min, float max )       { *val = Min( Max( *val, min ), max ); }

inline void Clamp( double *val, double min, double max )    { *val = Min( Max( *val, min ), max ); }

inline void Clamp( Vector2 *val, Vector2 const &min, Vector2 const &max ) {
    val->x = Min( Max( val->x, min.x ), max.x );
    val->y = Min( Max( val->y, min.y ), max.y );
}

inline void Clamp( Vector3 *val, Vector3 const &min, Vector3 const &max ) {
    val->x = Min( Max( val->x, min.x ), max.x );
    val->y = Min( Max( val->y, min.y ), max.y );
    val->z = Min( Max( val->z, min.z ), max.z );
}

inline void Clamp( Vector4 *val, Vector4 const &min, Vector4 const &max ) {
    val->x = Min( Max( val->x, min.x ), max.x );
    val->y = Min( Max( val->y, min.y ), max.y );
    val->z = Min( Max( val->z, min.z ), max.z );
    val->w = Min( Max( val->w, min.w ), max.w );
}

/*------------------------------------------------------------------------------
wrap to the specified range [min, max)
------------------------------------------------------------------------------*/
inline void Wrap( float *val, float min, float max ) {
    float range = max - min;
    float temp = *val;

    while( temp >= max ) temp -= range;
    while( temp < min  ) temp += range;
    *val = temp;
}

/*------------------------------------------------------------------------------
swap two values
------------------------------------------------------------------------------*/
inline void Swap( float *a, float *b ) {
    float originalA = *a;
    *a = *b;
    *b = originalA;
}

inline void Swap( double *a, double *b ) {
    double originalA = *a;
    *a = *b;
    *b = originalA;
}

inline void Swap( int8_t *a, int8_t *b ) {
    int8_t originalA = *a;
    *a = *b;
    *b = originalA;
}

inline void Swap( uint8_t *a, uint8_t *b ) {
    uint8_t originalA = *a;
    *a = *b;
    *b = originalA;
}

inline void Swap( int16_t *a, int16_t *b ) {
    int16_t originalA = *a;
    *a = *b;
    *b = originalA;
}

inline void Swap( uint16_t *a, uint16_t *b ) {
    uint16_t originalA = *a;
    *a = *b;
    *b = originalA;
}

inline void Swap( int32_t *a, int32_t *b ) {
    int32_t originalA = *a;
    *a = *b;
    *b = originalA;
}

inline void Swap( uint32_t *a, uint32_t *b ) {
    uint32_t originalA = *a;
    *a = *b;
    *b = originalA;
}

inline void Swap( int64_t *a, int64_t *b ) {
    int64_t originalA = *a;
    *a = *b;
    *b = originalA;
}

inline void Swap( uint64_t *a, uint64_t *b ) {
    uint64_t originalA = *a;
    *a = *b;
    *b = originalA;
}

inline void Swap( Quaternion *a, Quaternion *b ) {
    Quaternion originalA = *a;
    *a = *b;
    *b = originalA;
}

inline void Swap( Matrix *a, Matrix *b ) {
    Matrix originalA = *a;
    *a = *b;
    *b = originalA;
}

inline void Swap( Vector4 *a, Vector4 *b ) {
    Vector4 originalA = *a;
    *a = *b;
    *b = originalA;
}

inline void Swap( Vector3 *a, Vector3 *b ) {
    Vector3 originalA = *a;
    *a = *b;
    *b = originalA;
}

inline void Swap( Vector2 *a, Vector2 *b ) {
    Vector2 originalA = *a;
    *a = *b;
    *b = originalA;
}

/*------------------------------------------------------------------------------
calculate the 32 bit CRC of a null-terminated string
------------------------------------------------------------------------------*/
inline uint32_t Crc32( char const *string ) {
    char const *str = string;
    uint32_t crc = 0xffffffff;
    while( *str ) {
        crc = crcLookup[ ( uint8_t )( ( crc & 0xff ) ^ *str ) ] ^ ( crc >> 8 );
        ++str;
    }
    return ~crc;
}

/*------------------------------------------------------------------------------
calculate the 32 bit CRC of a data buffer
------------------------------------------------------------------------------*/
inline uint32_t Crc32( void const *data, unsigned size ) {
    uint8_t const *ptr = (uint8_t const*)data;
    uint32_t crc = 0xffffffff;
    for( uint32_t i = 0; i < size; ++i, ++ptr ) {
        crc = crcLookup[ ( uint8_t )( ( crc & 0xff ) ^ *ptr ) ] ^ ( crc >> 8 );
    }
    return ~crc;

#if 0
    // code to generate the lookup table
    uint32_t table[ 256 ];
    for( uint32_t i = 0; i < 256; ++i ) {
        uint32_t loopByte = i;
        uint32_t value = 0;

        for( uint32_t j = 0; j < 8; ++j ) {
            value = ( value >> 1 ) ^ ( ( ( ( value ^ loopByte ) & 1 ) != 0 ) * 0xedb88320 );
            loopByte >>= 1;
        }

        table[ i ] = value;
    }
#endif
}

#endif // UTILITY_H