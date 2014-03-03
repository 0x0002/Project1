#ifndef PROFILER_NODE_H
#define PROFILER_NODE_H

#include "Stopwatch.h"  // timing

/*------------------------------------------------------------------------------
CPU timing
------------------------------------------------------------------------------*/
class ProfilerNode {
public:
    // constructor
    inline ProfilerNode( char const *name ) : m_name( name ) { BeginScope(); m_stopwatch.Start(); }

    // destructor
    inline ~ProfilerNode() { m_stopwatch.Stop(); EndScope(); }

private:
    // tell the profiler we've started timing
    void BeginScope();

    // report timing results to the profiler
    void EndScope();

private:
    Stopwatch   m_stopwatch;    // used to time a duration
    char const *m_name;         // name of the profiled section
};

#endif // PROFILER_NODE_H