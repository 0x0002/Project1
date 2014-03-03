#include "Precompiled.h"
#include "Assert.h"
#include <iostream>		// printf stuff
#include <D3D11.h>	    // need to include before DxErr.h

enum { kBufferSize = 1024 };

char g_errorDescription[ kBufferSize ] = { 0 };

/*--------------------------------------------------------------------------
gets a DirectX error message
--------------------------------------------------------------------------*/
char const* GetErrorDescription( long error ) {
    FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                   NULL,
                   error,
                   MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
                   (LPSTR)&g_errorDescription,
                   kBufferSize,
                   NULL );
    return g_errorDescription;
}

/*--------------------------------------------------------------------------
reports an error
--------------------------------------------------------------------------*/
void ReportError( const char *errorType,
                  const char *file,
                  int line,
                  const char *format,
                  ... ) {
	char outputWindowMessage[ kBufferSize ];
	char messageBoxMessage[ kBufferSize ];

	// print out the file and line in visual studio format so the error
	// can be double clicked in the output window file(line): error
	int offset = sprintf_s( outputWindowMessage, "%s(%d): ", file , line );	
	if ( format != NULL ){	
		va_list args;
		va_start( args, format );
		vsnprintf_s( outputWindowMessage + offset,
					 kBufferSize - offset,
					 _TRUNCATE,
					 format,
					 args );
		vsnprintf_s( messageBoxMessage,
					 kBufferSize,
					 _TRUNCATE,
					 format,
					 args );
		va_end( args );
	}
	else{
		strcpy_s( outputWindowMessage + offset,
				  kBufferSize - offset,
				  "No error message" );

		strcpy_s( messageBoxMessage,
				  kBufferSize,
				  "No error message" );
	}

	// print to visual studio output window
	OutputDebugString( outputWindowMessage );
	OutputDebugString( "\n" );

	// display a message box
	MessageBox( NULL, messageBoxMessage, errorType, 0 );
}