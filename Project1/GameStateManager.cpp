#include "Precompiled.h"
#include "GameStateManager.h"
#include "GameStateIncludes.h"      // all game state classes
#include "GameStateStrings.h"       // c-string of component names
#include "GameStateHashes.h"        // Crc32 of component names

// global singleton
GameStateManager g_gameStateManager;

/*------------------------------------------------------------------------------
load the initial game state
------------------------------------------------------------------------------*/
void GameStateManager::Initialize() {
    // placement new to initialize vtable pointer
    m_hashToTypeMap = new ( PermanentAllocate( sizeof( HashToTypeMap ) ) ) HashToTypeMap();

    // allocate one instance of each game state
    for( uint32_t i = 0; i < GameState::kCount; ++i ) {

        // add lookup key/value pair to the map
        ( *m_hashToTypeMap )[ Crc32( GameState::String[ i ] ) ] = (GameState::Type)i;

	    #define DECLARE_GAME_STATE( state ) \
            case GameState::k##state: { \
                m_states[ i ] = new ( PermanentAllocate( sizeof( state ) ) ) state(); \
                break; \
            }

	    switch( i ) {
		    #include "GameStateDeclarations.h"
            default: AssertAlways( "invalid game state type" ); break;	
	    };
	    #undef DECLARE_GAME_STATE
    }

    m_currentState = GameState::kEngineStartup;
    m_nextState    = GameState::kEngineStartup;

    m_states[ GameState::kEngineStartup ]->Initialize();
}

/*------------------------------------------------------------------------------
deinitialize
------------------------------------------------------------------------------*/
void GameStateManager::Deinitialize() {
    m_states[ m_currentState ]->Deinitialize();

    // explicit destructor call because we allocated with placement new
    m_hashToTypeMap->~HashToTypeMap();
}

/*------------------------------------------------------------------------------
update the current game state
------------------------------------------------------------------------------*/
void GameStateManager::Update( float dt ) {
    if( m_currentState != m_nextState ) {
        // transition to the next state
        m_states[ m_currentState ]->Deinitialize();
        m_states[ m_nextState ]->Initialize();
        m_currentState = m_nextState;
    }

    m_states[ m_currentState ]->Update( dt );
}

/*------------------------------------------------------------------------------
set the state to transition to after engine startup completes
------------------------------------------------------------------------------*/
void GameStateManager::SetInitialState( GameState::Type state ) {
    Assert( m_currentState == GameState::kEngineStartup, "invalid startup state" );
    ( (EngineStartup*)m_states[ GameState::kEngineStartup ] )->SetNextState( state );
}

/*------------------------------------------------------------------------------
transition to a new state at the end of the frame
------------------------------------------------------------------------------*/
void GameStateManager::ChangeState( GameState::Type state ) {
    Assert( state != m_currentState, "must transition to a different game state" );
    m_nextState = state;
}

/*------------------------------------------------------------------------------
game state name/type/hash conversions
------------------------------------------------------------------------------*/
GameState::Type GameStateManager::GetType  ( char const      *name     ) { return GetType( Crc32( name ) ); }
GameState::Type GameStateManager::GetType  ( uint32_t         nameHash ) { HashToTypeMap::iterator i = m_hashToTypeMap->find( nameHash ); return i != m_hashToTypeMap->end() ? i->second : GameState::kInvalid; }
uint32_t        GameStateManager::GetHash  ( char const      *name     ) { return Crc32( name ); }
uint32_t        GameStateManager::GetHash  ( GameState::Type  type     ) { return GameState::Hash[ type ]; }
char const*     GameStateManager::GetString( GameState::Type  type     ) { return GameState::String[ type ]; }
char const*     GameStateManager::GetString( uint32_t         nameHash ) { return GameState::String[ GetType( nameHash ) ]; }