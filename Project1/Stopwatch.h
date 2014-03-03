#ifndef STOPWATCH_H
#define STOPWATCH_H

/*------------------------------------------------------------------------------
used for timing durations
------------------------------------------------------------------------------*/
class Stopwatch {
public:
	// start timing
	void Start();

	// finish timing
	void Stop();

	// get the timed duration
	inline float GetDuration() const { return m_duration; }

private:
	long long   m_start;		// start time
	float       m_duration;		// timed duration in milliseconds
};

#endif // STOPWATCH_H