#ifndef GAME_WINDOW_MESSAGE_HANDLERS_H
#define GAME_WINDOW_MESSAGE_HANDLERS_H

#include "Graphics.h"           // resizing
#include "Device.h"
#include "InputManager.h"       //

/*------------------------------------------------------------------------------
WM_DESTROY
------------------------------------------------------------------------------*/
LRESULT WM_DESTROY_HANDLER( HWND, WPARAM, LPARAM ) {
    PostQuitMessage( 0 );
    return 0;
}

/*------------------------------------------------------------------------------
WM_SIZE
------------------------------------------------------------------------------*/
LRESULT WM_SIZE_HANDLER( HWND, WPARAM wParam, LPARAM lParam ) {
    UnreferencedVariable( lParam );

    g_gameWindow.m_minimized = ( wParam == SIZE_MINIMIZED );

     // check fullscreen state
    BOOL fullscreen;
    IDXGIOutput *target = NULL;
    g_device.GetSwapChain()->GetFullscreenState( &fullscreen, &target );
    if( target ) target->Release();

    g_gameWindow.m_fullscreen = fullscreen != 0;   

    // update the window size
    RECT windowRect;
    GetWindowRect( g_gameWindow.m_windowHandle, &windowRect );
    g_gameWindow.m_windowSize.x = (int16_t)( windowRect.right - windowRect.left ); 
    g_gameWindow.m_windowSize.y = (int16_t)( windowRect.bottom - windowRect.top );

    // update the client size
    g_gameWindow.m_clientSize = Point( LOWORD( lParam ), HIWORD( lParam ) );

    g_graphics.Resize( g_gameWindow.m_resolution );
    return 0;
}

/*------------------------------------------------------------------------------
WM_PAINT
------------------------------------------------------------------------------*/
LRESULT WM_PAINT_HANDLER( HWND hWnd, WPARAM, LPARAM ) {
    HDC deviceContext;
	PAINTSTRUCT paintStruct;

	deviceContext = BeginPaint( hWnd, &paintStruct );
	EndPaint( hWnd, &paintStruct );
    return 0;
}

/*------------------------------------------------------------------------------
WM_MOUSEMOVE
------------------------------------------------------------------------------*/
LRESULT WM_MOUSEMOVE_HANDLER( HWND, WPARAM, LPARAM lParam ) {
    g_inputManager.OnMouseMove( LOWORD( lParam ), HIWORD( lParam ) );
    return 0;
}

/*------------------------------------------------------------------------------
WM_MOUSEWHEEL
------------------------------------------------------------------------------*/
LRESULT WM_MOUSEWHEEL_HANDLER( HWND, WPARAM wParam, LPARAM ) {
    g_inputManager.OnMouseWheel( HIWORD( wParam ) );
    return 0;
}

/*------------------------------------------------------------------------------
WM_LBUTTONDOWN
------------------------------------------------------------------------------*/
LRESULT WM_LBUTTONDOWN_HANDLER( HWND, WPARAM, LPARAM ) {
    g_inputManager.OnMouseButton( Button::kLeft, true );
    return 0;
}

/*------------------------------------------------------------------------------
WM_MBUTTONDOWN
------------------------------------------------------------------------------*/
LRESULT WM_MBUTTONDOWN_HANDLER( HWND, WPARAM, LPARAM ) {
    g_inputManager.OnMouseButton( Button::kMiddle, true );
    return 0;
}

/*------------------------------------------------------------------------------
WM_RBUTTONDOWN
------------------------------------------------------------------------------*/
LRESULT WM_RBUTTONDOWN_HANDLER( HWND, WPARAM, LPARAM ) {
    g_inputManager.OnMouseButton( Button::kRight, true );
    return 0;
}

/*------------------------------------------------------------------------------
WM_LBUTTONUP
------------------------------------------------------------------------------*/
LRESULT WM_LBUTTONUP_HANDLER( HWND, WPARAM, LPARAM ) {
    g_inputManager.OnMouseButton( Button::kLeft, false );
    return 0;
}

/*------------------------------------------------------------------------------
WM_MBUTTONUP
------------------------------------------------------------------------------*/
LRESULT WM_MBUTTONUP_HANDLER( HWND, WPARAM, LPARAM ) {
    g_inputManager.OnMouseButton( Button::kMiddle, false );
    return 0;
}

/*------------------------------------------------------------------------------
WM_RBUTTONUP
------------------------------------------------------------------------------*/
LRESULT WM_RBUTTONUP_HANDLER( HWND, WPARAM, LPARAM ) {
    g_inputManager.OnMouseButton( Button::kRight, false );
    return 0;
}

/*------------------------------------------------------------------------------
WM_KEYDOWN
------------------------------------------------------------------------------*/
LRESULT WM_KEYDOWN_HANDLER( HWND, WPARAM wParam, LPARAM ) {
    g_inputManager.OnKey( (Key::Type)wParam, true );
    return 0;
}

/*------------------------------------------------------------------------------
WM_KEYUP
------------------------------------------------------------------------------*/
LRESULT WM_KEYUP_HANDLER( HWND, WPARAM wParam, LPARAM ) {
    g_inputManager.OnKey( (Key::Type)wParam, false );
    return 0;
}

// macro to simplify message handler syntax
#define REGISTER_MESSAGE_HANDLER( message ) ( *m_messageHandlers )[ message ] = message##_HANDLER

/*------------------------------------------------------------------------------
add message handler functions to a map
------------------------------------------------------------------------------*/
void GameWindow::RegisterMessageHandlers() {
    REGISTER_MESSAGE_HANDLER( WM_DESTROY );
    REGISTER_MESSAGE_HANDLER( WM_SIZE );
    REGISTER_MESSAGE_HANDLER( WM_PAINT );
    REGISTER_MESSAGE_HANDLER( WM_MOUSEMOVE );
    REGISTER_MESSAGE_HANDLER( WM_MOUSEWHEEL );
    REGISTER_MESSAGE_HANDLER( WM_LBUTTONDOWN );
    REGISTER_MESSAGE_HANDLER( WM_MBUTTONDOWN );
    REGISTER_MESSAGE_HANDLER( WM_RBUTTONDOWN );
    REGISTER_MESSAGE_HANDLER( WM_LBUTTONUP );
    REGISTER_MESSAGE_HANDLER( WM_MBUTTONUP );
    REGISTER_MESSAGE_HANDLER( WM_RBUTTONUP );
    REGISTER_MESSAGE_HANDLER( WM_KEYDOWN );
    REGISTER_MESSAGE_HANDLER( WM_KEYUP );
}

#endif // GAME_WINDOW_MESSAGE_HANDLERS_H