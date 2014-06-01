#pragma once

//#include <boost/noncopyable.hpp>

#include <sstream>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include <DescentEngine/src/PropertyTreeSupport.h>
#include <DescentEngine/src/EntityEngine/EntityFactory.h>
#include <DescentEngine/src/XmlTools.h>

#include <DescentEngine/src/Engines.h>
//#include <DescentLogic/src/GameState.h>
#include <DescentLogic/src/Entities/WallEntity.h>
#include <DescentLogic/src/Entities/WallCollisionEntity.h>
#include <DescentLogic/src/Entities/FloorEntity.h>

#include "Exceptions/LevelParsingException.h"

class GameState;

struct LevelTile {

	LevelTile(Vector2 const& cen, Vector2 const& rep, bool col, std::string const& type,
			std::string const& ent, bool collideOnJump) :
			Center(cen), Repeat(rep), Collides(col), CollidesOnJump(collideOnJump), Type(type), Entity(ent) {
	}

	Vector2 Center;
	Vector2 Repeat;
	bool Collides;
	bool CollidesOnJump;
	std::string Type;
	std::string Entity;

};

typedef std::string LevelConnection;
/*
 struct LevelConnection {

 };*/

struct LevelFragment {
	std::vector<LevelTile> Tiles;
	Vector2 Size;
	std::string Name;
	std::vector<LevelConnection> ConnectionsIn;
	std::vector<LevelConnection> ConnectionsOut;
};

class LevelLoader {
public:

	void loadFragment(std::string const& json, LevelFragment & frag);

	template<class TGameState, class TEntFactory>
	void applyFragment(LevelFragment const& frag, TGameState & gs, TEntFactory & fact, int offsetY = 0) {

		for (auto const& tile : frag.Tiles) {

			for (int repX = 0; repX < tile.Repeat.x(); repX++) {
				for (int repY = 0; repY < tile.Repeat.y(); repY++) {
					//entFactory
					Entity * ent = nullptr;
					EntityTemplate const& entTemplate = gs.getEngines().entityEngine().getTemplate(
							tile.Entity);
					const Rectangle2 entSize = entTemplate.VisualSize;
					const Rectangle2 boundSize = entTemplate.BoundingBox;
					const Vector2 offset(repX * entSize.width(), repY * entSize.height() + offsetY);

					if (tile.Type == "WallEntity") {
						ent = fact.template createFromTemplateName<WallEntity>(tile.Entity,
								tile.Center + offset);
					}
					if (tile.Type == "FloorEntity") {
						ent = fact.template createFromTemplateName<FloorEntity>(tile.Entity,
								tile.Center + offset);
					}

					if (ent != nullptr) {
						gs.addStaticEntity(ent);

						// create blocking ?
						if (tile.Collides == 1) {
							// todo : give correct angle here
							auto col = fact.template createNonVisual<WallCollisionEntity>(tile.Center + offset,
									boundSize, LayerPriorityBottom, tile.CollidesOnJump);

							if (!tile.CollidesOnJump) {
								col->setCollisionGroup(GameCollisionGroups::StaticsJumpable);
								col->setCollisionMask(GameCollisionGroups::StaticsJumpable_CollidesWith);
							}

							gs.addStaticEntity(col);

						}
					} else {
						logging::Fatal() << "Unknown entity type: " << tile.Type;
					}

				}
			}
		}
	}
};

typedef std::vector<std::string> LevelFragmentsName;

class LevelFactory: boost::noncopyable {
public:

	LevelFactory(Engines & engines, LevelFragmentsName const& fragments, ResourceEngine & re) :
			m_engines(engines), m_fact(engines), m_lastFragment(nullptr), m_currentLayer(0) {

		LevelLoader loader;

		for (std::string const & fr : fragments) {
			LevelFragment frag;
			frag.Name = fr;
			const std::string xmlInp = re.loadLevel(fr);

			try {
				loader.loadFragment(xmlInp, frag);
			} catch (LevelParsingException & ex) {
				logging::Error() << "Level " << fr << " can not be parsed";
			}
			m_fragments.push_back(frag);
		}
	}

	void loadFragment(GameState & gs, std::string const& fragName);

	void nextLayers(GameState & gs, unsigned int amount);

	bool canConnect(LevelFragment const& prevFrag, LevelFragment const& nextFrag) const;

	void collisionTestLevel(GameState & gs);

	void renderTestLevel(GameState & gs);

	void reset();

private:
	Engines m_engines;
	EntityFactory m_fact;
	LevelFragment const* m_lastFragment;
	std::vector<LevelFragment> m_fragments;
	unsigned int m_currentLayer;
};
