#ifndef GAME_STATE_TYPES_H
#define GAME_STATE_TYPES_H

namespace GameState {

#define DECLARE_GAME_STATE( state ) k##state,

// enumeration of all game state types
enum Type {
	#include "GameStateDeclarations.h"
	kCount,
	kInvalid
};

#undef DECLARE_GAME_STATE

}

#endif // GAME_STATE_TYPES_H