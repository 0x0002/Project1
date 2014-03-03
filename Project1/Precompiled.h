// constants
#define kPi			3.14159265358979323846f
#define k2Pi		6.28318530717958647692f
#define kPiBy2		( kPi / 2.0f )
#define kPiBy4      ( kPi / 4.0f )
#define kInvPi		( 1.0f / kPi )
#define kInv2Pi		( 1.0f / k2Pi )
#define kEpsilon    0.00001f
#define kSqrtTwo    1.41421356237309504880f

#define kTempTextLength 512     // use this when you create temporary text buffers for convenience

#include <stdint.h>             // uint32_t, etc.
#include <new.h>                // placement new
#include <cmath>                // math stuff
#include "Utility.h"            // miscellaneous stuff
#include "Assert.h"             // assertions
#include "Memory.h"             // allocators
#include "Quaternion.h"         // quaternions
#include "Matrix.h"             // 4x4 matrix
#include "Matrix33.h"           // 3x3 matrix
#include "Matrix22.h"           // 2x2 matrix
#include "Vector4.h"            // 4d vector
#include "Vector3.h"            // 3d vector
#include "Vector2.h"            // 2d vector
#include "Point.h"              // pair of int16s
#include "UPoint.h"             // pair of uint16s
#include "Rect.h"               // four int16s
#include "Print.h"              // Print functions
#include "Random.h"             // random number generator
#include "FilePath.h"           // file path wrapper

#define STRINGIFY( L )          #L
#define MAKE_STRING( M, L )     M( L )
#define LINE                    MAKE_STRING( STRINGIFY, __LINE__ )

/*------------------------------------------------------------------------------
prints a clickable message to the VS output window during compilation
------------------------------------------------------------------------------*/
#define Todo( todo )            __pragma(message( __FILE__ "(" LINE "): TODO: " todo ))

/*------------------------------------------------------------------------------
measures the duration of a function
------------------------------------------------------------------------------*/
#define ProfileFunction() ProfilerNode ___function_profiler___ = ProfilerNode( __FUNCTION__ )

/*------------------------------------------------------------------------------
use this to call member functions through a function pointer
object should not be a pointer. dereference it if it is.

example:
    Object *object;
    void ( Object::*function )( int a ) = &Object::Foo;
    CallMemberFn( *object, function )( 324521 );
------------------------------------------------------------------------------*/
#define CallMemberFn( object, function ) ( ( object ).*( function ) )

/*------------------------------------------------------------------------------
removes a warning
------------------------------------------------------------------------------*/
#define UnreferencedVariable( variable ) ( variable )

/*------------------------------------------------------------------------------
offset a pointer
------------------------------------------------------------------------------*/
#define Offset( ptr, offset ) (void*)( (char*)ptr + offset )

/*------------------------------------------------------------------------------
pointer a - pointer b
------------------------------------------------------------------------------*/
#define OffsetOf( a, b ) ( (char*)a  - (char*)b )

/*------------------------------------------------------------------------------
number of elements in an array
------------------------------------------------------------------------------*/
#define ArraySize( a ) ( sizeof( a ) / sizeof( *a ) )

/*------------------------------------------------------------------------------
unit conversion from degree to radian
------------------------------------------------------------------------------*/
#define ToRadian( degree ) ( degree * ( kPi / 180.0f ) )

/*------------------------------------------------------------------------------
unit conversion from radian to degree
------------------------------------------------------------------------------*/
#define ToDegree( radian ) ( radian * ( 180.0f / kPi ) )