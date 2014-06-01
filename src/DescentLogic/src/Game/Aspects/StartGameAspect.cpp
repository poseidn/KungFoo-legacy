#include "StartGameAspect.h"

#include <DescentEngine/src/EntityEngine/EntityFactory.h>
#include <DescentEngine/src/Visuals/TextVisual.h>
#include <DescentEngine/src/StateEngine/StateChangeInfo.h>

#include "../GameState.h"
#include "../../Entities/PlayerEntity.h"
#include "../../Entities/EnemyEntity.h"
#include "../../Common/ChangeToGameInfo.h"
#include "../Util/SafePlacement.h"
#include "../../LevelFactory.h"

void StartGameAspect::init(GameState & gs) {
	gs.slotActivateState.subscribe([=] ( GameState & g, StateChangeInfoPtr const& ci )
	{	this->onActivate( g, ci);});

	gs.slotDeactivateState.subscribe([=] ( GameState & g )
	{	this->onDeactivate( g);});

	gs.slotOpenMenu.subscribe([=] ( GameState & g )
	{	this->onOpenMenu( g);});
}

void StartGameAspect::onActivate(GameState & gs, StateChangeInfoPtr const& stateInfo) {
	logging::Info() << "bootstrapping game world";

	auto toGame = std::dynamic_pointer_cast < ChangeToGameInfo > (stateInfo);
	assert(toGame);

	logging::Info() << "Starting game with " << toGame->PlayerCount << " players";

	gs.setPlayerCount(toGame->PlayerCount);

	gs.reset();
	gs.getLevelFactory()->reset();

	EntityFactory fact(gs.getEngines());

	// call this, once the screen transform is available
	gs.getEngines().renderEngine().setCameraLocation(Vector3(0.0f, 0.0f, 0.0f));
	gs.getLevelFactory()->nextLayers(gs, 0);

	// create as many player entities as needed
	int i = 0;
	for (PlayerData & pd : gs.getPlayers()) {
		std::stringstream playerEntName;
		playerEntName << "player" << (i + 1);

		Vector2 playerStart = Vector2(5.0f + float(i), 6.0f);

		const float maxPlacementRadius = 10.0f;
		const float playerCollisionRadius = 0.7f;

		std::pair<bool, Vector2> bestPlacement = game_util::findSafePlacement(playerStart,
				playerCollisionRadius, maxPlacementRadius, gs.getEngines(), gs);

		if (!bestPlacement.first) {
			logging::Fatal() << "B*I*G Problem: cannot find a placement space for player entity";
		}

		PlayerEntity * ent = fact.createMultiVisual<PlayerEntity>(playerEntName.str(), bestPlacement.second,
				LayerPriorityTopMost);
		ent->changeActiveVisual(gs.getEngines(), DescentTextureIds::Walk_0);
		pd.Entity = ent;
		gs.addEntity(ent);

		i++;
	}

	TexturePtr textTex = gs.getEngines().resourceEngine().loadImage("textChars.png");

	Vector2 topMost = gs.getEngines().renderEngine().getScreenTransform().screenSizeInTiles();
	Vector2 locationBox = Vector2(3.2f, topMost.y() - 1.7f);

	TextVisual * textV = new TextVisual(gs.getEngines().renderEngine().getScreenTransform(), textTex,
			locationBox, "Score: 0");
	gs.getEngines().renderEngine().addTextVisual(textV);
	gs.setTextScore ( textV );

}

void StartGameAspect::onDeactivate(GameState & gs) {

	// remove all entities from the engines
	for (PlayerData & pd : gs.getPlayers()) {

		gs.getEngines().entityEngine().removeEntity(static_cast<Entity *>(pd.Entity), gs.getEngines());
		pd.Entity = nullptr;
	}

	auto enemyList = gs.getEnemies();
	for (EnemyEntity * en : enemyList) {
		gs.getEngines().entityEngine().removeEntity(static_cast<Entity *>(en), gs.getEngines());
	}
	gs.getEnemies().clear();
	gs.getEngines().entityEngine().cleanAllStatic(gs.getEngines());

	gs.getEngines().renderEngine().removeTextVisual(gs.getTextScore());
	delete gs.getTextScore();
}

void StartGameAspect::onOpenMenu(GameState & gs) {
	logging::Info() << "Opening Menu now";
	gs.requestStateChange("menu");
}

