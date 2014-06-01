#include "DieAspect.h"

#include <sstream>
#include <algorithm>

#include <DescentEngine/src/Log.h>
#include <DescentEngine/src/Util.h>
#include <DescentEngine/src/SoundEngine/SoundEngine.h>
#include <DescentEngine/src/EntityEngine/EntityFactory.h>
#include <DescentEngine/src/EntityEngine/SingleVisualEntity.h>

#include <DescentLogic/src/Entities/PlayerEntity.h>
#include <DescentLogic/src/Config/GameRules.h>
#include <DescentLogic/src/Game/Util/SafePlacement.h>

#include "../GameState.h"

#include "../../Common/GameToMenuInfo.h"

void DieAspect::init(GameState & gameState) {
	gameState.slotStep.subscribe([=] ( GameState & g, float t)
	{	this->step(g,t);});

	gameState.slotActivateState.subscribe([=] ( GameState & gs, StateChangeInfoPtr const& ci )
	{
		EntityFactory fact(gs.getEngines());
		for ( size_t i = 0; i < GameRules::MaxPlayers; i++ ) {

			SingleVisualEntity * ent = fact.createFromTemplateName <SingleVisualEntity>("player_arrow",
					// intial location not important, will be reset anyway
			Vector2(0.0f, 0.0f));
	ent->getActiveVisual().get().setVisible(false);

	m_marker[i] = ent;
	gs.getEngines().entityEngine().addEntity(ent);
}
});

	gameState.slotDeactivateState.subscribe([=] ( GameState & gs )
	{
		for ( size_t i = 0; i < GameRules::MaxPlayers; i++ ) {
			gs.getEngines().entityEngine().removeEntity( m_marker[i], gs.getEngines() );
		}
	});

}

void DieAspect::updateMarker(PlayerData & pd, bool belowHorizon, Vector3 const& camLocation) {

	SingleVisualEntity * marker = m_marker.at(pd.Id);
	marker->getActiveVisual().get().setVisible(belowHorizon);

	if (belowHorizon) {
		// move the marker just to the lower part of the screen
		marker->setMoveIntent(Vector2(pd.Entity->getPosition().x(), camLocation.y() + 1.3f));
	}

}

void DieAspect::step(GameState & gs, float deltaT) {

	for (PlayerData & pd : gs.getPlayers()) {
		PlayerEntity * pent = pd.Entity;
		if (pent == nullptr) {
			std::cout << "missing ent";
			continue;
		}

		// check if the player has been moved outside of the game area
		const Vector3 camVec = gs.getEngines().renderEngine().getCameraLocation();

		util::initMap(m_deathInfo, pd.Id);
		DeathInfo & dInfo = m_deathInfo[pd.Id];

		// don't check this stuff for players which are already dead
		if (((pent->getPosition().y() + GameRules::DeadPositionTolerance) < camVec.y()) && (!pd.IsDead)) {
			// in the dead zone
			updateMarker(pd, true, camVec);

			// need to start the timer ?
			if (dInfo.TimeDead < 0.0f) {

				dInfo.TimeDead = GameRules::DeadTolerance;
				auto soundRes = gs.getEngines().resourceEngine().loadSound("dead_siren");
				m_dieWarningSound = gs.getEngines().soundEngine().playSound(soundRes);
			} else {
				dInfo.TimeDead -= deltaT;

				if (dInfo.TimeDead < 0.0f) {
					// totally dead
					logging::Info() << "Player died !";
					stopSiren(gs);

					playerDied(gs, pd);
					updateMarker(pd, false, camVec);
				}
			}

			/*logging::Info() << "Die die die, pl: "
			 << gs.getPlayerEntity()->getPosition().y() << " cam "
			 << camVec.y();*/
		} else {
			if (dInfo.TimeDead > 0.0) {
				updateMarker(pd, false, camVec);
				dInfo.TimeDead = -1.0f;
				// todo: stop the dead siren
				stopSiren(gs);
			}
		}

		// don't care about the position
		// is this player dead and we can respawn ?
		if ((gs.getGameMode() == GameMode::Coop) && (pd.IsDead) && (pd.RespawnTime > 0.0f)) {
			pd.RespawnTime -= deltaT;
			if (pd.RespawnTime <= 0.0f) {
				playerRespawn(gs, pd);
			}

		}
	}
}

void DieAspect::playerRespawn(GameState & gs, PlayerData & pd) {

	logging::Info() << "Respawning player id " << pd.Id;

	const Vector3 camVec = gs.getEngines().renderEngine().getCameraLocation();
	auto respawnPos = Vector2(4.0f, camVec.y() + 8.0f);
	auto safePos = game_util::findSafePlacement(respawnPos, 2.0f, 4.0f, gs.getEngines(), gs);

	// if not succesful, don't place now, but try the next round
	if (safePos.first) {
		pd.Entity->setMoveIntent(safePos.second);
		pd.Entity->skipCollsionOneRound();
		pd.IsDead = false;
		pd.RespawnTime = -1.0f;
	}
}

void DieAspect::playerDied(GameState & gs, PlayerData & pd) {
	assert(!pd.IsDead);

// is this the only player which was alive ?
	const size_t playersAlive = std::count_if(gs.getPlayers().begin(), gs.getPlayers().end(),
			[](PlayerData const& p ) {return ! p .IsDead;});

	pd.IsDead = true;

	if (playersAlive == 1) {
		// game over, last player alive
		gotoGameOver(gs);
	} else {
		// setup for respawn
		pd.RespawnTime = GameRules::RespawnTime;
		// using the static const for respawn with the ostream directly results in a linker error...
		const float resp = GameRules::RespawnTime;
		logging::Info() << "Starting respawn with " << resp << " s for player " << pd.Id;
	}

}

void DieAspect::gotoGameOver(GameState & gs) {
	auto changePtr = std::make_shared<GameToMenuInfo>();
	changePtr->PlayerDied = true;
	changePtr->PlayerCount = gs.getPlayerCount();
// fix
	if (gs.getGameMode() == GameMode::Coop) {
		changePtr->FinalScore = gs.getTotalScorePlayerSum();
	} else {
		logging::Fatal() << "This game mode is not supported for final score";
	}

	gs.requestStateChange("menu", changePtr);
}

void DieAspect::stopSiren(GameState & gs) {
	gs.getEngines().soundEngine().stopPlay(m_dieWarningSound, 1000.0f);
}
