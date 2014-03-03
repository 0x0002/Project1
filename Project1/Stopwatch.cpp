#include "Precompiled.h"
#include "Stopwatch.h"
#include "Time.h"           // system timing

/*------------------------------------------------------------------------------
start timing
------------------------------------------------------------------------------*/
void Stopwatch::Start() {
	// get the current time
	m_start = GetCounter();
}

/*------------------------------------------------------------------------------
finish timing
------------------------------------------------------------------------------*/
void Stopwatch::Stop() {
	m_duration = (float)GetTimeChange( m_start );
}