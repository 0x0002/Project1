#include "Precompiled.h"
#include "Print.h"
#include <windows.h>	    // windows.h header
#include <iostream>		    // printf
#include "Lock.h"           // for thread safety

Lock g_printLock;
enum { 
    kVSOutputBufferSize = 1024 * 100,
    kConsoleBufferSize  = 1024 * 100
};

void va_Print( char const *format, va_list args );
void va_PrintLine( char const *format, va_list args );
void va_PrintConsole( char const *format, va_list args );
void va_PrintConsole( PrintColor::Type color, char const *format, va_list args );
void va_PrintLineConsole( char const *format, va_list args );
void va_PrintLineConsole( PrintColor::Type color, char const *format, va_list args );

/*------------------------------------------------------------------------------
print to visual studio output window
------------------------------------------------------------------------------*/
void Print( char const *format, ... ) {
	// get the variadic argument
	va_list args;
	va_start( args, format );
    va_Print( format, args );
	va_end( args );
}

/*------------------------------------------------------------------------------
print to visual studio output window
------------------------------------------------------------------------------*/
void va_Print( char const *format, va_list args ) {
	char buffer[ kVSOutputBufferSize ];

	// print to a buffer that can be passed to OutputDebugString()
	vsprintf_s( buffer, kVSOutputBufferSize, format, args );

	// print to visual studio output window
	OutputDebugString( buffer );
}

/*------------------------------------------------------------------------------
print to visual studio output window. a newline is added to the string
------------------------------------------------------------------------------*/
void PrintLine( char const *format, ... ) {
	// get the variadic argument
	va_list args;
	va_start( args, format );
    va_PrintLine( format, args );
	va_end( args );
}

/*------------------------------------------------------------------------------
print to visual studio output window. a newline is added to the string
------------------------------------------------------------------------------*/
void va_PrintLine( char const *format, va_list args ) {
	char buffer[ kVSOutputBufferSize ];

	// print to a buffer that can be passed to OutputDebugString()
	vsprintf_s( buffer, kVSOutputBufferSize, format, args );

    // add a newline character
    strcat_s( buffer, "\n" );

	// print to visual studio output window
	OutputDebugString( buffer );
}

/*------------------------------------------------------------------------------
print to the console window
------------------------------------------------------------------------------*/
void PrintConsole( char const *format, ... ) {
	// get the variadic argument
	va_list args;
	va_start( args, format );
    va_PrintConsole( format, args );
	va_end( args );
}

/*------------------------------------------------------------------------------
print to the console window
------------------------------------------------------------------------------*/
void va_PrintConsole( char const *format, va_list args ) {
	g_printLock.Acquire();
	vprintf( format, args );
	g_printLock.Release();
}

/*------------------------------------------------------------------------------
print colored text to the console window
------------------------------------------------------------------------------*/
void PrintConsole( PrintColor::Type color, char const *format, ... ) {
	// get the variadic argument
	va_list args;
	va_start( args, format );
    va_PrintConsole( color, format, args );
	va_end( args );
}

/*------------------------------------------------------------------------------
print colored text to the console window
------------------------------------------------------------------------------*/
void va_PrintConsole( PrintColor::Type color, char const *format, va_list args ) {
	g_printLock.Acquire();

	// get a handle to stdout
	HANDLE hstdout = GetStdHandle( STD_OUTPUT_HANDLE );

	// store the current stdout state
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo( hstdout, &csbi );

	// change the color
	SetConsoleTextAttribute( hstdout, (WORD)color );

	// print
	vprintf( format, args );

	// set to original color
	SetConsoleTextAttribute( hstdout, csbi.wAttributes );

	g_printLock.Release();
}

/*------------------------------------------------------------------------------
print to the console window. a newline is added to the string
------------------------------------------------------------------------------*/
void PrintLineConsole( char const *format, ... ) {
	// get the variadic argument
	va_list args;
	va_start( args, format );
    va_PrintLineConsole( format, args );
	va_end( args );
}

/*------------------------------------------------------------------------------
print to the console window. a newline is added to the string
------------------------------------------------------------------------------*/
void va_PrintLineConsole( char const *format, va_list args ) {
    char buffer[ kConsoleBufferSize ];
	
    g_printLock.Acquire();

	// print to a buffer that can be passed to printf
	vsprintf_s( buffer, kConsoleBufferSize, format, args );

    // add a newline character
    strcat_s( buffer, "\n" );

	// print
    printf( "%s", buffer );

	g_printLock.Release();
}

/*------------------------------------------------------------------------------
print colored text to the console window. a newline is added to the string
------------------------------------------------------------------------------*/
void PrintLineConsole( PrintColor::Type color, char const *format, ... ) {
	// get the variadic argument
	va_list args;
	va_start( args, format );
    va_PrintLineConsole( color, format, args );
	va_end( args );
}

/*------------------------------------------------------------------------------
print colored text to the console window. a newline is added to the string
------------------------------------------------------------------------------*/
void va_PrintLineConsole( PrintColor::Type color, char const *format, va_list args ) {
    char buffer[ kConsoleBufferSize ];
	
    g_printLock.Acquire();

	// get a handle to stdout
	HANDLE hstdout = GetStdHandle( STD_OUTPUT_HANDLE );

	// store the current stdout state
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo( hstdout, &csbi );

	// change the color
	SetConsoleTextAttribute( hstdout, (WORD)color );

	// print to a buffer that can be passed to printf
	vsprintf_s( buffer, kConsoleBufferSize, format, args );

    // add a newline character
    strcat_s( buffer, "\n" );

	// print
    printf( "%s", buffer );

	// set to original color
	SetConsoleTextAttribute( hstdout, csbi.wAttributes );

	g_printLock.Release();
}