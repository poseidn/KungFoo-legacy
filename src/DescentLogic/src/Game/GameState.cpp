#include "GameState.h"

#include <DescentEngine/src/ResourceEngine/ResourceEngine.h>
#include <DescentEngine/src/Visuals/TextVisual.h>
#include <DescentEngine/src/Physics/PhysicsEngine.h>
#include <DescentEngine/src/Log.h>

#include <DescentLogic/src/Entities/EnemyEntity.h>
#include <DescentLogic/src/Entities/BloodEntity.h>
#include <DescentLogic/src/LevelFactory.h>

GameState::GameState(Engines & eg) :
		StateBase(), m_timestamp(0.0f), m_engines(eg) {
	/*
	 level setup:

	 a: indoor - two rooms, seperated by wall
	 b: indoor - one room
	 c: outdoor - wide gras, no doors
	 d: outdoor - middle bridge
	 e: outdoor - wide gras with middle doors
	 f: outdoor - with walls at the bottom, this is the one which connects indoor to outdoor
	 */

	LevelFragmentsName frags = { "indoor_wood1", "indoor_wood2", "indoor_wood3", "outdoor_gras1",
			"outdoor_gras2", "outdoor_gras3", "outdoor_bridge1" };

	m_factory.reset(new LevelFactory(m_engines, frags, m_engines.resourceEngine()));

	// our own timestamp calculation
	slotStep.subscribe([] ( GameState & gs, float dt ) {gs.m_timestamp += dt;});
}

void GameState::logSignalPerformance() const {
	// only for the step slot right now
#ifdef DESCENT_SIGNAL_PROFILE
	slots::TimingResultList tres = slotStep.getTimingResultList();

	logging::Info() << "Step Signal:";
	for (auto const& item : tres) {
		logging::Info() << "Subscriber name : " << item.first << " average time (ms)" << item.second;
	}
#endif
}

void GameState::addEnemyEntity(EnemyEntity * entEnemy) {
	m_enemies.push_back(entEnemy);
	addEntity(static_cast<Entity *>(entEnemy));
}

void GameState::addFragment(BloodEntity * frag) {
	m_fragments.push_back(frag);
	addEntity(static_cast<Entity*>(frag));
}

void GameState::removeEnemyEntity(EnemyEntity * entEnemy) {
	m_enemies.remove(entEnemy);
}

int GameState::getTotalScorePlayerSum() const {
	int total = 0;
	for (PlayerData const & pd : getPlayers()) {
		total += pd.TotalScore;
	}

	return total;
}

void GameState::setPlayerCount(int count) {
	m_playerData.resize(count);
	for (int i = 0; i < count; i++) {
		// reset player data
		m_playerData[i] = PlayerData();
		m_playerData[i].Id = i;
	}
}

void GameState::reset() {
	for (PlayerData & pd : m_playerData) {
		pd.TotalScore = 0;
	}
	m_timestamp = 0.0f;
}

void GameState::updateScoreDisplay() {

	if (getGameMode() == GameMode::Coop) {
		std::stringstream scoreStr;

		// sum up score
		scoreStr << "Score: " << getTotalScorePlayerSum();
		m_textScore->setText(scoreStr.str(), getEngines().renderEngine().getScreenTransform());
	} else {
		logging::Fatal() << "Scores for this game mode is not supported";
	}

}

void GameState::clearAllStaticEntities() {
	getEngines().entityEngine().cleanAllStatic(getEngines());
}

void GameState::loadTestFragment() {
	LevelLoader loader;
	EntityFactory entFact(getEngines());
//LevelFactory fact(getEngines());

	const std::string xmlInp = getEngines().resourceEngine().loadLevel(m_levelName);

	try {
		clearAllStaticEntities();
		logging::Info() << " try loading level fragment " << m_levelName;

		LevelFragment frag;
		loader.loadFragment(xmlInp, frag);
		loader.applyFragment(frag, *this, entFact);
		logging::Info() << "done";
	} catch (LevelParsingException & ex) {
		logging::Error() << "Level " << m_levelName << " can not be parsed";
	}
}

void GameState::addEntity(Entity * ent) {
	getEngines().entityEngine().addEntity(ent);
	getEngines().physicsEngine().registerEntity(ent);
	slotEntityAdded.signal(*this, ent);
}

void GameState::addStaticEntity(Entity * ent) {
	getEngines().entityEngine().addStaticEntity(ent);
	getEngines().physicsEngine().registerEntity(ent);
	slotEntityAdded.signal(*this, ent);
}

void GameState::addEntity(Entity * ent, std::string const& name) {
	getEngines().entityEngine().addEntity(ent, name);
	slotEntityAdded.signal(*this, ent);

}
