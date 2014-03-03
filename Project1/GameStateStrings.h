#ifndef GAME_STATE_STRINGS_H
#define GAME_STATE_STRINGS_H

// only include this in GameStateManager.cpp

namespace GameState {

#define DECLARE_GAME_STATE( state ) "k"#state,

// array of all game state string names
char const *String[] = {
	#include "GameStateDeclarations.h"
	"kCount",
	"kInvalid"
};

#undef DECLARE_GAME_STATE

}

#endif // GAME_STATE_STRINGS_H