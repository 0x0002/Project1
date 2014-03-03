#ifndef ENGINE_STARTUP_H
#define ENGINE_STARTUP_H

#include "GameStateBase.h"        //  base class
#include "GameStateTypes.h"       // 

/*------------------------------------------------------------------------------
engine startup
------------------------------------------------------------------------------*/
class EngineStartup : public GameStateBase {
public:
    virtual void Initialize();
    virtual void Update( float dt );

    inline void SetNextState( GameState::Type nextState ) { m_nextState = nextState; }

private:
    float m_countdown;
    GameState::Type m_nextState;
};

#endif // ENGINE_STARTUP_H