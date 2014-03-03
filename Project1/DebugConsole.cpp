#include "Precompiled.h"
#include "DebugConsole.h"
#include <windows.h>	// windows.h header
#include <iostream>		// console window stuff
#include <io.h>			// _open_osfhandle
#include <fcntl.h>		// _O_TEXT

enum { 
    kConsoleSizeX = 80,
    kConsoleSizeY = 500,
};

/*------------------------------------------------------------------------------
open the debug console window
------------------------------------------------------------------------------*/
void OpenDebugConsole(){
	// allocate a console window
	if( !AllocConsole() ) {
		// a console is already allocated
		return;
	}

	// set the number of lines allowed in the console window
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &consoleInfo);

    consoleInfo.dwSize.X = kConsoleSizeX;
    consoleInfo.dwSize.Y = kConsoleSizeY;
	SetConsoleScreenBufferSize( GetStdHandle( STD_OUTPUT_HANDLE ), consoleInfo.dwSize);

	// redirect unbuffered STDOUT to the console	
	int osfHandle = _open_osfhandle( (intptr_t)GetStdHandle( STD_OUTPUT_HANDLE ), _O_TEXT );
	FILE *fp = _fdopen( osfHandle, "w" );
	*stdout = *fp;
	setvbuf( stdout, NULL, _IONBF, 0 );
	std::ios::sync_with_stdio();
}

/*------------------------------------------------------------------------------
close the debug console window
------------------------------------------------------------------------------*/
void CloseDebugConsole(){
	FreeConsole();
}