#ifndef GAME_STATE_HASHES_H
#define GAME_STATE_HASHES_H

// only include this in GameStateManager.cpp

namespace GameState {

#define DECLARE_GAME_STATE( state ) Crc32( "k"#state ),

// hash of all game state types
uint32_t const Hash[] = {
	#include "GameStateDeclarations.h"
	Crc32( "kCount"   ),
	Crc32( "kInvalid" )
};

#undef DECLARE_GAME_STATE

}

#endif // GAME_STATE_HASHES_H