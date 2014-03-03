#ifndef TIME_H
#define TIME_H

#include <windows.h>        // windows.h header

/*------------------------------------------------------------------------------
get the current time in ticks
------------------------------------------------------------------------------*/
inline long long GetCounter() {
	LARGE_INTEGER counter;
	QueryPerformanceCounter( &counter );
	return counter.QuadPart;
}

/*------------------------------------------------------------------------------
get the current CPU frequency
------------------------------------------------------------------------------*/
inline long long GetFrequency() {
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency( &frequency );
	return frequency.QuadPart;
}

/*------------------------------------------------------------------------------
get the difference between a start time and the current time in seconds
------------------------------------------------------------------------------*/
inline double GetTimeChange( long long start ) {
	unsigned long long end = GetCounter();
	return (double)( end - start ) / (double)GetFrequency();
}

#endif // TIME_H