#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include "SingletonBase.h"		            // base class
#include <map>                              // map container
#include "GameWindowMessageHandlerFriend.h" // macro to declare friend message hander functions

/*------------------------------------------------------------------------------
manages the game window and Windows messages
------------------------------------------------------------------------------*/
class GameWindow : public SingletonBase< GameWindow > {
public:
    // these message handlers need access to private data members
    FRIEND_HANDLER( WM_SIZE );

    friend LResult __stdcall MessageHandler( Hwnd hWnd,
                                             unsigned int msg,
                                             WParam wParam,
                                             LParam lParam );
	// create the game window
	void Initialize();
    
    // close and cleanup the game window
	void Deinitialize();

	// handle operating system messages. returns false when the program should exit
	bool HandleMessages();

    // display the game window
    void ShowGameWindow();

    // hide the game window
    void HideGameWindow();
    
    // set the screen resolution and fullscreen state
    void SetResolutionAndFullscreenState( Point const &resolution, bool fullscreen );

    // get the current mouse position. used during initialization before a WM_MOUSEMOVE is received
    Point GetMousePosition();

    // accessors
    inline Hwnd GetWindowHandle()           const { return m_windowHandle;   }
    inline Point const& GetWindowPosition() const { return m_windowPosition; }
    inline Point const& GetWindowSize()     const { return m_windowSize;     }
    inline Point const& GetClientSize()     const { return m_clientSize;     }
    inline Point const& GetResolution()     const { return m_resolution;     }
    inline bool IsActive()                  const { return m_active;         }
    inline bool IsMinimized()               const { return m_minimized;      }
    inline bool IsFullscreen()              const { return m_fullscreen;     }

private:
    // register a window class so a window can be created
    void RegisterWindowClass();

    // create but do not display the game window
    Hwnd CreateGameWindow();

    // add message handling functions to a map
    void RegisterMessageHandlers();

public:
    Hwnd            m_windowHandle;       // handle to the game window
    Point           m_windowPosition;     // screen coordinates of the window
    Point           m_windowSize;         // usable area plus borders
    Point           m_clientSize;         // usable are
    Point           m_resolution;         // current rendering resolution
    bool            m_active;             // true if the window is active
    bool            m_minimized;          // true if the window is minimized
    bool            m_fullscreen;         // true if the window is fullscreen

    typedef LResult ( *MessageHandlerFn )( Hwnd hWnd, WParam wParam, LParam lParam );
    typedef std::map< uint32_t, MessageHandlerFn > MessageHandlerMap;
    MessageHandlerMap *m_messageHandlers; // list of all message handler function pointers
};

// global instance
extern GameWindow g_gameWindow;

#endif // GAME_WINDOW_H