#include "StatusAspect.h"

#include <sstream>
#include <DescentEngine/src/Log.h>
#include <DescentEngine/src/ResourceEngine/ResourceEngine.h>

#include "../GameState.h"
#include "../../Config/GameRules.h"

void StatusAspect::init(GameState & gameState) {
	gameState.slotStep.subscribe([=] ( GameState & gs, float deltaT)
	{
		// do we have to view the respawn ?
			bool showRespawn = false;
			float smallestRespawn = GameRules::RespawnTime * 2.0f;
			for ( PlayerData & pd : gs.getPlayers( ))
			{
				if ( pd.IsDead) {
					smallestRespawn = std::min ( smallestRespawn, pd.RespawnTime);
					showRespawn = true;
				}
			}

			if ( showRespawn)
			{	std::stringstream sText;
				sText << "Respawn in " << int(smallestRespawn);
				m_respawnCounter->setText( sText.str(), gs.getEngines().renderEngine().getScreenTransform() );
			}
			m_respawnCounter->setVisible( showRespawn);
		});

	gameState.slotActivateState.subscribe([=] ( GameState & gs, StateChangeInfoPtr const& ci )
	{
		TexturePtr textTex = gs.getEngines().resourceEngine().loadImage("textChars.png");

		Vector2 topMost = gs.getEngines().renderEngine().getScreenTransform().screenSizeInTiles();
		Vector2 locationBox = Vector2(topMost.x() * 0.2f, 1.0f);

		TextVisual * textV = new TextVisual(gs.getEngines().renderEngine().getScreenTransform(), textTex,
				locationBox, "Respawn in 13");
		gs.getEngines().renderEngine().addTextVisual(textV);
		m_respawnCounter = textV;
	});

	gameState.slotDeactivateState.subscribe([=] ( GameState & gs )
	{
		gs.getEngines().renderEngine().removeTextVisual(m_respawnCounter);
		delete m_respawnCounter;
	});

}

