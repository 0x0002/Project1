#ifndef GAME_STATE_BASE_H
#define GAME_STATE_BASE_H

/*------------------------------------------------------------------------------
defines the game state interface
------------------------------------------------------------------------------*/
class GameStateBase {
public:
    GameStateBase() {}
    virtual ~GameStateBase() {}

    virtual void Initialize() {}
    virtual void Update( float dt ) { UnreferencedVariable( dt ); }
    virtual void Deinitialize() {}
};

#endif // GAME_STATE_BASE_H