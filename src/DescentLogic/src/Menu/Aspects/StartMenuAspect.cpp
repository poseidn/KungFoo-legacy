#include "StartMenuAspect.h"

#include <DescentEngine/src/Visuals/TextVisual.h>
#include <DescentEngine/src/EntityEngine/EntityFactory.h>
#include <DescentEngine/src/EntityEngine/SingleVisualEntity.h>
#include <DescentEngine/src/StateEngine/StateChangeInfo.h>
#include <DescentEngine/src/Visuals/VisualSupport.h>

#include <DescentLogic/src/Menu/MenuState.h>
#include "../../Common/GameToMenuInfo.h"

void StartMenuAspect::init(MenuState & gs) {
	gs.slotActivateState.subscribe([&] ( MenuState & gs, StateChangeInfoPtr const& changeInfo )
	{
		logging::Info() << "setting up Menu state" << std::endl;
		EntityFactory fact(gs.getEngines());

		// call this, once the screen transform is available
			gs.getEngines().renderEngine().setCameraLocation(Vector3(0.0f, 0.0f, 0.0f));

			/*Entity * guyEnt = fact.create<  SingleVisualEntity>("bullet", Vector2(4.0f, 4.0f));
			 gs.getEngines().entityEngine().addEntity(guyEnt, &gs.getManagedEntityList());
			 */
			auto toMenu = std::dynamic_pointer_cast< GameToMenuInfo >(changeInfo);

			gs.bootstrapMenu();

			ScreenTransform const& trans = gs.getEngines().renderEngine().getScreenTransform();

			// we could also have no GameToMenuInfo, on the first start of the game
			bool playerDied = false;

			// we might not have a change info at all
			if ( toMenu ) {
				if ( toMenu->PlayerDied)
				playerDied = true;
			}

			if (playerDied )
			{
				Vector2 gameOverPos(4.0f, 6.0f);
				TexturePtr textTex = gs.getEngines().resourceEngine().loadImage("textChars.png");
				m_gameOverVisual = new TextVisual(trans, textTex,
						gameOverPos, "Game Over");
				gs.getEngines().renderEngine().addTextVisual(m_gameOverVisual);

				Vector2 scorePos(4.0f, 5.0f);

				std::stringstream sScore;
				sScore << "Score: " << toMenu->FinalScore;

				m_scoreVisual = new TextVisual(trans, textTex,
						scorePos,sScore.str());
				gs.getEngines().renderEngine().addTextVisual(m_scoreVisual);

				m_delayedGameOver = 1.0f;

			} else {
				/*
				 Vector2 gameOverPos(0.0f, 6.0f);
				 TexturePtr textTex = gs.getEngines().resourceEngine().loadImage("textChars.png");
				 auto testTxt = new TextVisual(trans, textTex,
				 gameOverPos, "TEST");
				 gs.getEngines().renderEngine().addTextVisual(testTxt);*/

				EntityFactory fact(gs.getEngines());
				m_logoEntity = fact.createFromTemplateName< SingleVisualEntity>("game_logo",
						Vector2(1.0f, 1.0f));
				m_logoEntity->getActiveVisual().get().setIngame(trans, Vector2(0.0f, 5.0f), false);
				VisualSupport::placeXCenter( m_logoEntity, 0.5f, 0.75f, trans );
				gs.getEngines().entityEngine().addEntity(m_logoEntity , &gs.getManagedEntityList());
				/*
				 m_controlsEntity = fact.create<  SingleVisualEntity>("xbox-gamepad-labels", Vector2(13.0f, 7.0f));
				 gs.getEngines().entityEngine().addEntity( m_controlsEntity, &gs.getManagedEntityList());*/

				const float tableX = VisualSupport::positionXCenter( 0.5f, 0.5f, trans ).x() * 0.8f;

				const Vector2 posItemOne ( VisualSupport::positionXCenter( 0.28f, 0.0f, trans ).x(),
						VisualSupport::positionXCenter( 0.0f, 0.47f, trans ).y() );
				const Vector2 posItemTwo = posItemOne - Vector2(0.0f, 1.2f);

				MenuItem it("start", "Start Game",
						posItemOne);
				gs.addMenuItem(it);
				//VisualSupport::place( it.TextVis,  )

				it = MenuItem("exit", "Exit", Vector2(posItemTwo));
				gs.addMenuItem(it);

				gs.setCurrentItem(0);

				// generate all icons for the input devices
				DeviceList devList = gs.getEngines().inputEngine().availableInputDevices();
				logging::Info() << "Setting up menu for " << devList.size() << " input devices";

				const float maxTiles = trans.screenSizeInTiles().x();
				const size_t maxDevices = gs.getEngines().inputEngine().maxInputDevices();

				const float tilePerInputDev = maxTiles / float ( maxDevices);
				const float tileFreeSpace = tilePerInputDev * 0.5f;
				const float posYInputIcon = 2.5f;
				const float posYenableDisable = 1.1f;

				size_t curDevIndex = 0;
				if ( gs.getDeviceItems().size() == 0)
				{
					// add all available input devices, only done on the first execution
					for ( InputDevice const& iDev : devList )
					{
						MenuState::DeviceItem item;
						// todo: enable always the first controller, if there is only one
						item.m_isEnabled = false;
						item.m_type = iDev.Type;
						item.m_ContainerId = iDev.Index;

						gs.addDeviceItem( item );
					}
				}

				// setup graphics for the input items
				for ( auto & item : gs.getDeviceItems())
				{
					const float thisBeginTile = tilePerInputDev * float ( curDevIndex) + tileFreeSpace;

					const Vector2 posInputIcon (thisBeginTile, posYInputIcon);
					const Vector2 posInputEnable(thisBeginTile, posYenableDisable);

					std::string iconActive;
					std::string iconPassive;
					std::string iconDisable;
					std::string iconEnable;

					if ( item.m_type == InputDevice::TypeEnum::Keyboard ) {
						iconActive = "keyboard";
						iconPassive = "keyboard-passive";
						iconDisable = "keyboard-disable";
						iconEnable = "keyboard-enable";
					}
					else if ( item.m_type == InputDevice::TypeEnum::Gamepad ) {
						//todo: check for ouya / pc and use controllers accordingly

						/*						iconActive = "xbox-gamepad";
						 iconPassive = "xbox-gamepad-passive";
						 iconDisable = "xbox-gamepad-disable";
						 iconEnable = "xbox-gamepad-enable";*/

						iconActive = "ouya-gamepad";
						iconPassive = "ouya-gamepad-passive";
						iconDisable = "ouya-gamepad-disable";
						iconEnable = "ouya-gamepad-enable";
					}
					else
					{
						logging::Fatal() << "This input type is not supported by the menu";
					}

					item.m_Icon = fact.createFromTemplateName< SingleVisualEntity>(iconActive, posInputIcon);
					item.m_Icon->getActiveVisual().get().setIngame( trans, posInputIcon, false );
					gs.getEngines().entityEngine().addEntity( item.m_Icon, &gs.getManagedEntityList());

					item.m_IconPassive = fact.createFromTemplateName< SingleVisualEntity>(iconPassive, posInputIcon);
					item.m_IconPassive->getActiveVisual().get().setIngame( trans, posInputIcon, false );
					gs.getEngines().entityEngine().addEntity( item.m_IconPassive, &gs.getManagedEntityList());

					item.m_DisableText = fact.createFromTemplateName< SingleVisualEntity>(iconDisable, posInputEnable);
					item.m_DisableText->getActiveVisual().get().setIngame( trans, posInputEnable, false );
					gs.getEngines().entityEngine().addEntity( item.m_DisableText, &gs.getManagedEntityList());

					item.m_EnableText = fact.createFromTemplateName< SingleVisualEntity>(iconEnable, posInputEnable);
					item.m_EnableText->getActiveVisual().get().setIngame( trans, posInputEnable, false );
					gs.getEngines().entityEngine().addEntity( item.m_EnableText, &gs.getManagedEntityList());

					curDevIndex++;
				}

				gs.updateDevices();
			}

		});

	gs.slotDeactivateState.subscribe([&] ( MenuState & gs )
	{
		// remove all the entities which might still be registered with the render system
			logging::Info() << "closing menu";
			gs.getEngines().entityEngine().cleanManagedList(gs.getManagedEntityList(), gs.getEngines());
			gs.getManagedEntityList().clear();

			// entities have been cleaned via cleanManagedList
			// keep this for the next time we enter the menu
			//gs.m_deviceItems.clear();

			// kill text visuals
			for (MenuItem & it : gs.getMenuItemList()) {
				gs.getEngines().renderEngine().removeTextVisual(it.TextVis);
				delete it.TextVis;
			}
			gs.getMenuItemList().clear();

			if (m_gameOverVisual) {
				gs.getEngines().renderEngine().removeTextVisual(m_gameOverVisual);
				delete m_gameOverVisual;
				m_gameOverVisual = nullptr;
			}

			if (m_scoreVisual) {
				gs.getEngines().renderEngine().removeTextVisual(m_scoreVisual);
				delete m_scoreVisual;
				m_scoreVisual = nullptr;
			}

		});

	gs.slotStep.subscribe([&] ( MenuState & gs, float deltaT )
	{
		if ( m_delayedGameOver > 0.0f)
		{
			m_delayedGameOver -= deltaT;

			if ( m_delayedGameOver <= 0.0f )
			{
				MenuItem it("restart", "Continue", Vector2( 4.0f, 4.0f));
				gs.addMenuItem(it);
				/*it = MenuItem("exit", "Exit", Vector2( 4.0f, 3.0f));
				 gs.addMenuItem(it);*/

				gs.setCurrentItem(0);
			}
		}

	});

}
