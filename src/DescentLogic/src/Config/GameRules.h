#pragma once

class GameRules {
public:
	constexpr static float DeadTolerance = 2.0f;
	constexpr static float DeadPositionTolerance = 0.4f;
	constexpr static int PointsForKill = 5;
	constexpr static size_t MaxPlayers = 4;
	constexpr static float RespawnTime = 15.0f;

	// this will need 3 kicks to kill the enemy
	constexpr static size_t FighterLife = 10;

	constexpr static size_t ScorePerSecond = 1;
	// used to place enemies and player entities
	constexpr static float GamingPositionLowest = 1.0f;
	constexpr static float GamingPositionHighest = 14.0f;

	// ok yeah, combos baby !
	constexpr static size_t Combo5SingleCount = 5;
	constexpr static float Combo5SingleTimeFrame = 4.0f;
	constexpr static size_t Combo5SinglePoints = 30;

	constexpr static size_t Combo10SingleCount = 10;
	constexpr static float Combo10SingleTimeFrame = 6.0f;
	constexpr static size_t Combo10SinglePoints = 100;

	constexpr static size_t MaxBloodFragments = 200;

	/*
	 * not impemented yet
	 constexpr static size_t Combo10MultiCount = 10;
	 constexpr static float Combo10MultiTimeFrame = 6.0f;
	 constexpr static size_t Combo10MultiPoints = 50;
	 */
};
