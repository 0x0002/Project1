#include "Precompiled.h"
#include "Random.h"
#include <time.h>			// for seeding

static const uint32_t kRandMax = 0x7FFF;                // largest value that can be returned by Rand
static const float    kRandMaxInv = 1.0f / kRandMax;    // 1 / kRandMax

// used to generate random numbers
uint32_t g_rand = 0;

/*------------------------------------------------------------------------------
seeds the random number generator based on the current time
------------------------------------------------------------------------------*/
void SeedRand() {
	g_rand = (uint32_t)time( (time_t*)( 0 ) );
}

/*------------------------------------------------------------------------------
seeds the random number generator
------------------------------------------------------------------------------*/
void SeedRand( uint32_t seed ) {
	g_rand = seed ;
}

/*------------------------------------------------------------------------------
random int in the range [0,RAND_MAX]
linear congruential PRNG
------------------------------------------------------------------------------*/
uint32_t RandInt() {
	return ( ( ( g_rand = g_rand * 214013L + 2531011L ) >> 16 ) & kRandMax );
}

/*------------------------------------------------------------------------------
random float in the range [0,1]
------------------------------------------------------------------------------*/
float RandFloat() {
	return RandInt() * kRandMaxInv;
}

/*------------------------------------------------------------------------------
random float with gaussian distribution in the range [-1, 1]
------------------------------------------------------------------------------*/
float RandGaussian() {
	return ( 2 * ( RandFloat() + RandFloat() + RandFloat() ) - 3 ) * ( 1.0f / 3.0f );
}