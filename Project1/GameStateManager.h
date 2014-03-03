#ifndef GAME_STATE_MANAGER_H
#define GAME_STATE_MANAGER_H

#include "SingletonBase.h"          // base class
#include "GameStateTypes.h"         // type enum
#include <map>                      // std::map

// forward declaration
class GameStateBase;

/*------------------------------------------------------------------------------
tracks the current game state and handles state transitions
------------------------------------------------------------------------------*/
class GameStateManager {
public:
    // load the initial game state
    void Initialize();

    // deinitialize
    void Deinitialize();

    // update the current game state
    void Update( float dt );

    // set the state to transition to after engine startup completes
    void SetInitialState( GameState::Type state );

    // transition to a new state at the end of the frame
    void ChangeState( GameState::Type state );

    // game state name/type/hash conversions
    GameState::Type GetType  ( char const      *name     );
    GameState::Type GetType  ( uint32_t         nameHash );
    uint32_t        GetHash  ( char const      *name     );
    uint32_t        GetHash  ( GameState::Type  type     );
    char const*     GetString( GameState::Type  type     );
    char const*     GetString( uint32_t         nameHash );

private:
    GameState::Type   m_currentState;   // current game state
    GameState::Type   m_nextState;      // game state to transition to at the end of the frame

    GameStateBase    *m_states[ GameState::kCount ];   // game state instances

    typedef std::map< uint32_t, GameState::Type > HashToTypeMap;
    HashToTypeMap    *m_hashToTypeMap;  // used for component name/type/hash conversions
};

// global singleton
extern GameStateManager g_gameStateManager;

#endif // GAME_STATE_MANAGER_H