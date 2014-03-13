#include "Precompiled.h"
#include "GameLoop.h"

#include "Profiler.h"
#include "GameWindow.h"
#include "GameStateManager.h"
#include "Graphics.h"

#include "Time.h"

enum {
    kStackSize      = 1024 * 1024 * 32,
    kPermanentSize  = 1024 * 1024 * 64
};

// function declaration
void LoadSettings();

/*------------------------------------------------------------------------------
run the engine
------------------------------------------------------------------------------*/
void GameLoop(){ 
    #ifdef _DEBUG
        OpenDebugConsole();
    #endif

    PrintLineConsole( PrintColor::kBlue, "Project1" );

    // initialization
    // this initialization order is important. don't change the order.
    g_memoryManager.Initialize( kStackSize, kPermanentSize );
    g_profiler.Initialize();
    g_gameWindow.Initialize();
    g_graphics.Initialize();

    g_gameWindow.ShowGameWindow();      // this calls Windows message handler
    
    g_gameStateManager.Initialize();    // game state manager should initialize last


    g_gameWindow.SetResolutionAndFullscreenState( g_graphics.GetClosestSupportedResolution( Point( 1280, 720 ) ), false );
    g_gameStateManager.SetInitialState( GameState::kDefaultGameState );
    
	// main game loop
    double dt = 0;
    while( g_gameWindow.HandleMessages() ){    
        long long start = GetCounter();

        g_gameStateManager.Update( (float)dt );

        Sleep( 1 );
        dt = GetTimeChange( start );
    }

    // shutdown
    // deinitialize in opposite order of initialization
    g_graphics.PreDeinitialize();       // we need to exit fullscreen mode before doing any deinitialization

    g_gameStateManager.Deinitialize();  // game state manager should deinitialize first
    g_graphics.Deinitialize();
    g_gameWindow.Deinitialize();
    g_profiler.Deinitialize();
    g_memoryManager.Deinitialize();
}