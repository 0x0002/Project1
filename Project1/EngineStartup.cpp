#include "Precompiled.h"
#include "EngineStartup.h"
#include "GameStateManager.h"       // state transitions

/*------------------------------------------------------------------------------
initialization
------------------------------------------------------------------------------*/
void EngineStartup::Initialize() {
    PrintLineConsole( "EngineStartup" );
    m_nextState = GameState::kInvalid;
}

/*------------------------------------------------------------------------------
game logic update
------------------------------------------------------------------------------*/
void EngineStartup::Update( float dt ) {
    Assert( m_nextState != GameState::kInvalid, "you need to set an initial state using g_gameStateManager.SetInitialState()" );
	g_gameStateManager.ChangeState( m_nextState );
}