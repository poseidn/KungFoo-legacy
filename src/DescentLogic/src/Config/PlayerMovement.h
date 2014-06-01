#pragma once

#include <boost/math/constants/constants.hpp>

class PlayerMovement {
public:
	// maximum speed the player is allowed to move ( when not jumping )
	constexpr static float MaxSpeed = 13.0f;
	// the part which decais the movement which is dependent of the ratio between
	// current movement and max speed
	constexpr static float MovementDecayDynamic = 0.00f;
	// the constant part which is always removed from the player speed
	// the bigger, the more speed is removed
	constexpr static float MovementDecayStatic = 4.0f;

	// fixed to work with older boost version 
	constexpr static float MaxStuckAngle = /* boost::math::constants::pi<float>() */ 6.28 * 0.6f;

	constexpr static float AttackRange = 1.7f;
	constexpr static float AttackRangeSquared = AttackRange * AttackRange;

	// opens up a 180 degree field infront of the player to attack
	constexpr static float AttackAngle = 3.1415f * (1.0f / 2.0f);

	// how many enemies can be affected by one player kick at a time
	constexpr static size_t KickAffectCount = 2;

	constexpr static float JumpTime = 0.36f;
	constexpr static float JumpIncreaseSpeedFactor = 1.34f;

	constexpr static float HitRegenTime = 1.0f;

	//constexpr static float KickTime = 0.36f;
	constexpr static float KickTime = 0.11f;
	constexpr static int KickSubtractLife = 6;
	constexpr static float HitKickBackFactor = 0.7f;

	constexpr static float BikingCooldown = 1.0f;
	constexpr static float BikingTime = 2.5f;
	constexpr static float BikingMaxMovement = 0.5f;

	// the time which have to be waited afer one jump before the next
	// one can be performed
	constexpr static float JumpRegenerationTime = 0.06f;

	// the maximum number of fields which the player is allowed to advance infront of the camera
	// limits the players to not run "away" from the camera
	constexpr static float MaxAdvance = 11.0f;
};
