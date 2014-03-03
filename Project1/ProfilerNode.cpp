#include "Precompiled.h"
#include "ProfilerNode.h"
#include "Profiler.h"       // tracking results

/*------------------------------------------------------------------------------
tell the profiler we've started timing
------------------------------------------------------------------------------*/
void ProfilerNode::BeginScope() {
    g_profiler.BeginNode( m_name );
}

/*------------------------------------------------------------------------------
report timing results to the profiler
------------------------------------------------------------------------------*/
void ProfilerNode::EndScope() {
    g_profiler.EndNode( m_name, m_stopwatch.GetDuration() );
}