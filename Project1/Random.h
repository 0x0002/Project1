#ifndef RANDOM_H
#define RANDOM_H

// seeds the random number generator based on the current time
void SeedRand();

// seeds the random number generator
void SeedRand( uint32_t seed );

// random int in the range [0,RAND_MAX]
uint32_t RandInt();

// random float in the range [0,1]
float RandFloat();

// random float with gaussian distribution in the range [-1, 1]
float RandGaussian();

#endif // RANDOM_H