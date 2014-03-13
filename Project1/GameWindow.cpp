#include "Precompiled.h"
#include "GameWindow.h"

#include <Windows.h>            // windows stuff
#include <WindowsX.h>           // more windows stuff
#undef DrawText

#include "Device.h"             // resizing
//#include "UIManager.h"          // input handling and resizing

//#include "Graphics.h"

char const              *kClassName              = "window class name";
char const              *kWindowTitle            = "Project 1";
unsigned long const      kWindowStyle            = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;
uint32_t const           kWindowPositionXDefault = 0;
uint32_t const           kWindowPositionYDefault = 0;

// global instance
GameWindow g_gameWindow;

// main message handler
LRESULT CALLBACK MessageHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

/*------------------------------------------------------------------------------
create the game window
------------------------------------------------------------------------------*/
void GameWindow::Initialize() {
    m_resolution = Point( 1, 1 );
    m_active = false;
    m_minimized = false;
    m_fullscreen = false;

    // initialize the message handler map
    m_messageHandlers = new ( PermanentAllocate( sizeof( MessageHandlerMap ) ) ) MessageHandlerMap();
    RegisterMessageHandlers();

	// create a window
    RegisterWindowClass();
    m_windowHandle = CreateGameWindow();
}

/*------------------------------------------------------------------------------
close and cleanup the game window
------------------------------------------------------------------------------*/
void GameWindow::Deinitialize() {
    DestroyWindow( m_windowHandle );
    m_messageHandlers->~MessageHandlerMap();
}

/*------------------------------------------------------------------------------
handle operating system messages. returns false when the program should exit
------------------------------------------------------------------------------*/
bool GameWindow::HandleMessages() {
    static bool run = true;
	MSG msg;
	while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE) ) {
		if( msg.message == WM_QUIT ) {
			// WM_QUIT is never handled by the standard message handler
			run = false;
		}

		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
    return run;
}

/*------------------------------------------------------------------------------
register a window class so a window can be created
------------------------------------------------------------------------------*/
void GameWindow::RegisterWindowClass() {
    WNDCLASSEX wc;
    memset( &wc, 0, sizeof( wc ) );
	wc.cbSize = sizeof( wc );							    // size of the class
	wc.style = CS_HREDRAW | CS_VREDRAW;						// class style
	wc.lpfnWndProc = MessageHandler;						// a function pointer which should point to the procedure function
	wc.cbClsExtra = 0;										// the number of extra bytes you want to allocate for this window class structure
	wc.cbWndExtra = 0;										// the number of extra bytes you want to allocate for the window instance
	wc.hIcon = (HICON)LoadImage(NULL,"..\\..\\Textures\\red_logo_64.ico",IMAGE_ICON,0,0,LR_LOADFROMFILE);				// handle to the icon class which will be displayed on the top left part of the window
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);				// handle to the cursor class which will be used in this window class
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// handle to the class background brush. it can be a color value
	wc.lpszMenuName = NULL;									// pointer to a null terminated string which specifies the name of the menu resources used by this window class (if any)
	wc.lpszClassName = kClassName;					        // string that specifies the window class name. this is needed to create any window based on this window class

	RegisterClassEx( &wc );
}

/*------------------------------------------------------------------------------
create but do not display the game window
------------------------------------------------------------------------------*/
HWND GameWindow::CreateGameWindow() {

	// the window size must be larger than the screen size to to take the window borders, menus, etc. into acount
	RECT windowRect = { 0, 0, 1, 1 };
	AdjustWindowRect( &windowRect,
                      kWindowStyle,		// window style. must match whatever is passed below
					  FALSE);			// window doesn't have a menu
	
    m_windowSize.x = (int16_t)( windowRect.right - windowRect.left );
	m_windowSize.y = (int16_t)( windowRect.bottom - windowRect.top );

	// create the window
	return CreateWindowEx( NULL,
						   kClassName,		            // the class name we chose for the window class
						   kWindowTitle,				// the window caption
						   kWindowStyle,			    // the style of the window, which determines if the window will have a minimize/maximize buttons, if its frame is thick or not.. 
						   kWindowPositionXDefault,		// the X position of the top left corner of the window
						   kWindowPositionYDefault,		// the Y position of the top left corner of the window
                           m_windowSize.x,		        // the width of the window
						   m_windowSize.y,              // the height of the window
						   NULL,						// handle to the parent window (in case this was a child window)
						   NULL,						// handle to a menu (In case there is a menu for this window)
						   NULL,			            // instance of the module associated with the window
						   NULL );
}

/*------------------------------------------------------------------------------
display the game window
------------------------------------------------------------------------------*/
void GameWindow::ShowGameWindow() {
	ShowWindow( m_windowHandle, SW_SHOWDEFAULT );
	UpdateWindow( m_windowHandle );
}

/*------------------------------------------------------------------------------
hide the game window
------------------------------------------------------------------------------*/
void GameWindow::HideGameWindow() {
    ShowWindow( m_windowHandle, SW_HIDE );
}    

/*------------------------------------------------------------------------------
set the screen resolution and fullscreen state
------------------------------------------------------------------------------*/
void GameWindow::SetResolutionAndFullscreenState( Point const &resolution, bool fullscreen  ) {
    g_device.AcquireLock();

    bool updateResolution = m_resolution != resolution;

    // update these member variables before changing anything
    m_fullscreen = fullscreen;
    m_resolution = resolution;

    if( updateResolution ) {
        // ResizeTarget doesn't work if you're in fullscreen mode
        DxVerify( g_device.GetSwapChain()->SetFullscreenState( false, NULL ) );

        // if you are fullscreen and running at any resolution other than the monitor's native resolution,
        // you will see this warning:
        // DXGI Warning: IDXGISwapChain::Present: Fullscreen presentation inefficiencies incurred
        // this is unavoidable I think
        DXGI_MODE_DESC const *modeDesc = g_device.GetMatchingDisplayMode( resolution );
        DxVerify( g_device.GetSwapChain()->ResizeTarget( modeDesc ) );
    }
    DxVerify( g_device.GetSwapChain()->SetFullscreenState( fullscreen, NULL ) );

    g_device.ReleaseLock();
}

/*------------------------------------------------------------------------------
get the current mouse position. used during initialization before a WM_MOUSEMOVE is received
------------------------------------------------------------------------------*/
Point GameWindow::GetMousePosition() {
    POINT point;
    GetCursorPos( &point );
    ScreenToClient( m_windowHandle, &point );
    return Point( (int16_t)point.x, (int16_t)point.y );
}

/*------------------------------------------------------------------------------
message handlers
------------------------------------------------------------------------------*/
#include "GameWindowMessageHandlers.h"

/*------------------------------------------------------------------------------
message handler callback associated with the game window
------------------------------------------------------------------------------*/
LRESULT CALLBACK MessageHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) {
    GameWindow::MessageHandlerMap::iterator handler = g_gameWindow.m_messageHandlers->find( msg );
    if( handler != g_gameWindow.m_messageHandlers->end() ) {
        return handler->second( hWnd, wParam, lParam );
    }

    // if message isn't handled by this window, use the default handler
    return DefWindowProc( hWnd, msg, wParam, lParam );
}