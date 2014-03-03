#ifndef PROFILER_H
#define PROFILER_H

#include "SingletonBase.h"  // base class

// input callback function
void ToggleProfilerDrawing( float dt );

/*------------------------------------------------------------------------------
tracks CPU timing results from profiler nodes
------------------------------------------------------------------------------*/
class Profiler : public SingletonBase< Profiler > {
public:
    // initialize
    void Initialize();

    // deinitialize
    void Deinitialize();

    // reset the profiler
    void FrameStart();

    // draw results to the screen
    void DisplayResults();

    // reserve a result slot for a node
    void BeginNode( char const *name );

    // store timing results
    void EndNode( char const *name, float timing );

    // accessors
    inline bool GetDraw() const { return m_draw; }
    inline void SetDraw( bool draw ) { m_draw = draw; }

private:
    uint32_t m_stackDepth;      // number of nodes that are currently timing
    float   m_highestTiming;    // value of the node that is taking the longest

    struct Result {
        char const *m_name;     // name of the profiled section
        uint32_t    m_depth;    // stack depth when the result was reported
        float       m_timing;   // duration in seconds
    };

    Result      *m_results;     // array of results
    uint16_t     m_resultCount; //

    bool         m_draw;        //
};

// global instance
extern Profiler g_profiler;

#endif // PROFILER_H