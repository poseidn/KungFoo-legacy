#pragma once

class EnemyBehaviour {
public:

	// the squared distance to the player, where enemies will start to attack the player
	constexpr static float AttackSquaredDistancePlayer = 5.0f * 5.0f;
	constexpr static float AttackSquaredDistancePlayerRandomPart = 2.0f;

	constexpr static float StopAttackSquaredDistancePlayer = 10.0f * 10.0f;

	constexpr static float HitTime = 0.5f;
	constexpr static float MaxSpeed = 6.0f;
	constexpr static float AttacDelay = 0.5f;
	constexpr static float RegenerationAfterHit = 0.4f;

	constexpr static float ClosestApproach = 0.6f;
	constexpr static float KickDistancePlayer = 0.7f;

	/* problem when compiling with clang
	 * constexpr   int MinEnemiesPerSegment = 1;
	 constexpr int MaxEnemiesPerSegment = 6;*/

};
