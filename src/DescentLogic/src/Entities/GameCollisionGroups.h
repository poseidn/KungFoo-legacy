#pragma once

#include <DescentEngine/src/Physics/CollisionGroups.h>

struct GameCollisionGroups {

	static constexpr short Statics = 1 << 1;
	static constexpr short StaticsJumpable = 1 << 2;

	static constexpr short Characters = 1 << 3;
	static constexpr short CharactersJump = 1 << 4;

	// don't collide with other characters any more
	static constexpr short CharactersDead = 1 << 5;

	static constexpr short Statics_CollidesWith = CollisionGroups::Ground | Characters
			| CharactersJump | CharactersDead;

	static constexpr short StaticsJumpable_CollidesWith = CollisionGroups::Ground | Characters
			| CharactersDead;

	static constexpr short Characters_CollidesWith = CollisionGroups::Ground | Statics
			| StaticsJumpable | Characters | CharactersJump;

	static constexpr short CharactersJump_CollidesWith = CollisionGroups::Ground | Statics
			| Characters | CharactersJump;

	static constexpr short CharactersDead_CollidesWith = CollisionGroups::Ground | Statics
			| StaticsJumpable;
};
