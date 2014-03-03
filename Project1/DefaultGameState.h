#ifndef DEFAULT_GAME_STATE_H
#define DEFAULT_GAME_STATE_H

#include "GameStateBase.h"        //  base class
#include "GameStateTypes.h"       // 

/*------------------------------------------------------------------------------
default game state
------------------------------------------------------------------------------*/
class DefaultGameState : public GameStateBase {
public:
    virtual void Initialize() override;
    virtual void Update( float dt ) override;
};

#endif // DEFAULT_GAME_STATE_H