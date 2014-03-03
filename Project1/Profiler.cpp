#include "Precompiled.h"
#include "Profiler.h"
#include "Draw.h"           // drawing results
#include "GameWindow.h"     // screen resolution

// global instance
Profiler g_profiler;

enum {
    kMaxResults      = 50,

    kStartPosY       = 100,
    kYStep           = 15,

    kTimingPosX      = 10,
    kNamePosX        = 110,
};

/*------------------------------------------------------------------------------
initialize
------------------------------------------------------------------------------*/
void Profiler::Initialize() {
    m_results = (Result*)PermanentAllocate( sizeof( Result ) * kMaxResults );
    m_draw    = false;
}

/*------------------------------------------------------------------------------
deinitialize
------------------------------------------------------------------------------*/
void Profiler::Deinitialize() {
    // do nothing
}

/*------------------------------------------------------------------------------
reset the profiler
------------------------------------------------------------------------------*/
void Profiler::FrameStart() {
    m_stackDepth    = 0;
    m_resultCount   = 0;
    m_highestTiming = 0;
}

/*------------------------------------------------------------------------------
draw results to the screen
------------------------------------------------------------------------------*/
void Profiler::DisplayResults() {
    if( !m_draw ) return;

#if 0
    int16_t maxWidth = g_gameWindow.GetResolution().x - 2 * kTimingPosX;

    Result *result = m_results;
    uint32_t resultCount = m_resultCount;
    int16_t y = kStartPosY;
    for( uint32_t i = 0; i < resultCount; ++i, ++result, y += kYStep ) {
        // draw a bar, scaled so the result with highest timing extends to the screen edge
        int16_t width = (int16_t)( maxWidth * result->m_timing / m_highestTiming );
        int16_t left  = kTimingPosX;
        int16_t right = left + width;
        Draw::FilledRect( Point( left, y + 1 ), Point( right, y + kYStep - 1 ), Vector4( 0.2f, 0.8f, 0.2f, 0.7f ) );

        // draw the timing
        char text[ kTempTextLength ];
        sprintf_s( text, "%09.5f ms", result->m_timing * 1000.0f );
        Draw::Text2D( text, Point( kTimingPosX, y ) );

        // draw the name
        text[ 0 ] = '\0';
        for( uint32_t j = 0; j < result->m_depth; ++j ) strcat_s( text, "     " );
        strcat_s(text, result->m_name );
        Draw::Text2D( text, Point( kNamePosX, y ) );
    }
#endif
}

/*------------------------------------------------------------------------------
reserve a result slot for a node
------------------------------------------------------------------------------*/
void Profiler::BeginNode( char const *name ) {
    ++m_stackDepth;

    // check this name has already been used
    Result *result = m_results;
    uint32_t resultCount = m_resultCount;
    for( uint32_t i = 0; i < resultCount; ++i, ++result ) if( result->m_name == name ) return;

    // add the results to the array
    Assert( resultCount < kMaxResults, "increase kMaxResults" );
    result = &m_results[ resultCount ];
    result->m_depth  = m_stackDepth - 1;
    result->m_name   = name;
    result->m_timing = 0.0f;
    ++m_resultCount;
}

/*------------------------------------------------------------------------------
store timing results
------------------------------------------------------------------------------*/
void Profiler::EndNode( char const *name, float timing ) {
    --m_stackDepth;
    
    Result *result = m_results;
    uint32_t resultCount = m_resultCount;
    for( uint32_t i = 0; i < resultCount; ++i, ++result ) {
        if( result->m_name == name ) {
            // already have results for this name. update the timing
            result->m_timing += timing;
            m_highestTiming = Max( m_highestTiming, result->m_timing );
            return;
        }
    }
    AssertAlways();
}

/*------------------------------------------------------------------------------
input callback function
------------------------------------------------------------------------------*/
void ToggleProfilerDrawing( float ) { g_profiler.SetDraw( !g_profiler.GetDraw() ); }