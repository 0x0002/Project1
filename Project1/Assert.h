#ifndef ASSERT_H
#define ASSERT_H

/*--------------------------------------------------------------------------
use these to control assertion level
--------------------------------------------------------------------------*/
#define ENABLE_ASSERTIONS 1
#define ENABLE_WARNINGS 1

// gets a DirectX error message
char const* GetErrorDescription( long error );

// reports an error
void ReportError( const char *errorType,
                  const char *file,
				  int line,
				  const char *formatMessage = 0,
				  ... );

// break to the debugger using the MS specific function
#define TriggerBreakpoint() do { __debugbreak(); } while( 0, 0 )

/*--------------------------------------------------------------------------
an expression that does nothing
--------------------------------------------------------------------------*/
#ifndef VoidExpression
#define VoidExpression( expr ) \
	do{ \
		( void )sizeof( expr ); \
	}while( 0,0 )
#endif

#if ENABLE_ASSERTIONS
	/*--------------------------------------------------------------------------
	trigger a breakpoint if the expression fails
	used with directX functions
	--------------------------------------------------------------------------*/
	#define DxVerify( expr ) \
		do{ \
			long hr = ( expr ); \
			if( FAILED( hr ) ) { \
				ReportError( "DirectX Error", \
                             __FILE__, \
							 __LINE__, \
							 "DX Error %s", \
							 GetErrorDescription( hr ) ); \
				TriggerBreakpoint(); \
			} \
		} while( 0, 0 )

	/*--------------------------------------------------------------------------
	trigger a breakpoint if the expression is false
	--------------------------------------------------------------------------*/
	#define Assert( assumption, ... ) \
		do { \
			if( !( assumption ) ) { \
				ReportError( "Assert", \
                             __FILE__, \
							 __LINE__, \
							 ##__VA_ARGS__ ); \
				TriggerBreakpoint(); \
			} \
		} while( 0,0 )

    /*--------------------------------------------------------------------------
	always trigger a breakpoint
	--------------------------------------------------------------------------*/
    #define AssertAlways( ... ) \
		do { \
			ReportError( "Assert", \
                            __FILE__, \
							__LINE__, \
							##__VA_ARGS__ ); \
			TriggerBreakpoint(); \
		} while( 0,0 )

#else // if !ENABLE_ASSERTIONS
	#define DxVerify( expr ) do{ expr } while( 0, 0 )
	#define Assert( expr ) do{ VoidExpression( expr ); } while( 0, 0 )
    #define AssertAlways( expr ) do{ VoidExpression( expr ); } while( 0, 0 )
#endif // ENABLE_ASSERTIONS

#if ENABLE_WARNINGS
	/*--------------------------------------------------------------------------
	trigger a breakpoint if the expression is false
	--------------------------------------------------------------------------*/
	#define Warning( assumption, ... ) \
		do { \
			if( !( assumption ) ) { \
				ReportError( "Warning", \
                             __FILE__, \
							 __LINE__, \
							 ##__VA_ARGS__ ); \
				TriggerBreakpoint(); \
			} \
		} while( 0, 0 )

    /*--------------------------------------------------------------------------
	always trigger a breakpoint
	--------------------------------------------------------------------------*/
    #define WarningAlways( ... ) \
		do { \
			ReportError( "Warning", \
                         __FILE__, \
					     __LINE__, \
						 ##__VA_ARGS__ ); \
			TriggerBreakpoint(); \
		} while( 0, 0 )

#else // if !ENABLE_WARNINGS
    #define Warning( expr ) do{ VoidExpression( expr ); } while( 0, 0 )
    #define WarningAlways( expr ) do{ VoidExpression( expr ); } while( 0, 0 )
#endif // ENABLE_WARNINGS


#endif // ASSERT_H