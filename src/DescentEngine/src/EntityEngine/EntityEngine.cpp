#include "EntityEngine.h"

#include <algorithm>
#include <cmath>
#include <vector>

#include "Entity.h"
#include "EntityTemplate.h"
#include "../RenderEngine.h"
#include "MoveAccessClass.h"
#include "../Timing.h"

#include "../Pathfinding/Node.h"

EntityEngine::EntityEngine() {

}
EntityEngine::~EntityEngine() {
	dropNavigationNodes(m_pathfindingNodes);
}

void EntityEngine::addEntity(Entity * ent, ManagedEntityList * managedList) {
	m_entities.push_back(ent);
	if (managedList != nullptr) {
		managedList->push_back(ent);
	}
}

// todo: make sure this index is cleaned up properly
void EntityEngine::addEntity(Entity * ent, std::string const& name, ManagedEntityList * managedList) {
	addEntity(ent);
	m_entitiesIndex[name] = ent;
}

Entity * EntityEngine::getEntity(std::string const& name) {
	auto it = m_entitiesIndex.find(name);
	if (it == m_entitiesIndex.end()) {
		return nullptr;
	} else {
		return it->second;
	}

}

void EntityEngine::dropNavigationNodes(NodeVector & nodes) {
	nodes.clear();
}

bool EntityEngine::checkForCollisionObject(Vector2 const& position, const float radi) const {
	// check statics
	// this ensures that enemies keep their distance to walls and not get stuck
	const float posDelta = 3.0f;

	for (Entity * sEnt : getStaticEntities()) {
		if (sEnt->doesCollide()) {
			// some easy criteria to be faster ( especially on the ouya )
			if ((std::abs(position.y() - sEnt->getPosition().y()) < posDelta)
					&& (std::abs(position.x() - sEnt->getPosition().x()) < posDelta)) {

				const Vector2 conAtoB = sEnt->getPosition() - position;
				const float conDist = conAtoB.magSquared();
				if (conDist < (radi + sEnt->getCollisionRadius())) {
					// that's a collision
					return true;
				}
			}
		}
	}
	return false;
}

void EntityEngine::generatePathfindingNodes(NodeVector & nodes) {
	// lay a grid over the static entities
	Timing tm;

	const int resolutionFactor = 1;
	const float collisionMarginFactor = 2.5f;
	const float gridSize = 1.0f / float(resolutionFactor); // 0.5f;
	//const float neighbourDistance = sqrt(gridSize * gridSize * 2.0f) * 1.1f;

	float floatHighestY = 0;
	float floatLowestY = 100000000000.0f;
	for (Entity const* ent : getStaticEntities()) {
		floatHighestY = std::max(floatHighestY, ent->getPosition().y());
		floatLowestY = std::min(floatLowestY, ent->getPosition().y());
	}

	dropNavigationNodes(nodes);

	// protect against no entities
	if (getStaticEntities().size() == 0)
		return;

	assert(floatHighestY > floatLowestY);
	logging::Info() << "Generating navigation grid for lowY: " << floatLowestY << " highY: " << floatHighestY;

	const int highestY = int(std::ceil(floatHighestY));
	const int lowestY = int(std::floor(floatLowestY));
	const int spanY = highestY - lowestY;

	const float spanX = 15.0f;
	const int nodeCount = (spanX * resolutionFactor) * (spanY * resolutionFactor);

	nodes.resize(nodeCount);

	// with a resolution of 2 times the tile size
	const int sizeX = spanX * resolutionFactor;
	const int sizeY = (spanY * resolutionFactor);

	for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x++) {
			Vector2 pos(float(x) * gridSize, float(y) * gridSize + float(lowestY));

			// do only add if there is no collision object near by
			if (!checkForCollisionObject(pos, gridSize * collisionMarginFactor)) {
				const int thisNodeLocation = sizeX * y + x;
				NodePtr n = &nodes[thisNodeLocation];
				n->Location = pos;

				// connect to left
				if (x > 0) {
					// left same line
					n->Neighbours.push_back(&nodes[thisNodeLocation - 1]);

					// left lower
					if (y > 0) {
						n->Neighbours.push_back(&nodes[thisNodeLocation - 1 - sizeX]);
					}
					// left upper
					if (y < (sizeY - 1)) {
						n->Neighbours.push_back(&nodes[thisNodeLocation - 1 + sizeX]);
					}
				}
				if (x < (sizeX - 1)) {
					// right same line
					n->Neighbours.push_back(&nodes[thisNodeLocation + 1]);
					// right lower
					if (y > 0) {
						n->Neighbours.push_back(&nodes[thisNodeLocation + 1 - sizeX]);
					}
					// right upper
					if (y < (sizeY - 1)) {
						n->Neighbours.push_back(&nodes[thisNodeLocation + 1 + sizeX]);
					}
				}

				if (y > 0) {
					// lower one
					n->Neighbours.push_back(&nodes[thisNodeLocation - sizeX]);
				}
				if (y < (sizeY - 1)) {
					// upper one
					n->Neighbours.push_back(&nodes[thisNodeLocation + sizeX]);
				}
			}
		}
	}

	float dt = tm.end();
	logging::Info() << "Building the Pathfinding grid took " << dt << " seconds";
}

void EntityEngine::cleanManagedList(ManagedEntityList & managedList, Engines & e) {
	for (Entity * ent : managedList) {
		removeEntity(ent, e);
	}
}

void EntityEngine::cleanManagedStaticList(ManagedStaticEntityList & managedList, RenderEngine & re) {

}

void EntityEngine::addStaticEntity(Entity * ent, ManagedStaticEntityList *managedList) {
//im_stat
// add partitioned in y
	EntityList::iterator itAddPosition = getStaticEntitiesRegionStart(
			EntityRegion(ent->getPosition().y(), ent->getPosition().y()));

// insert is before itAddPosition
	m_staticEntities.insert(itAddPosition, ent);
}

void EntityEngine::executeMoveIntents() {
	for (EntityList::iterator it = getEntities().begin(); it != getEntities().end(); it++) {
		MoveAccessClass::applyMoveIntent(*(*it));
	}
}

void EntityEngine::updateDirtyEntities(ScreenTransform const& trans, VisualUpdatePairList & updateList) {
	for (EntityList::iterator it = getEntities().begin(); it != getEntities().end(); it++) {
		if ((*it)->isPositionDirty()) {
			updateList.push_back((*it)->updateVisual(trans));
			(*it)->unsetPositionDirty();
		}
	}
}

EntityList::iterator EntityEngine::getStaticEntitiesRegionStart(EntityRegion const& entRegion) {
	for (EntityList::iterator it = getStaticEntities().begin(); it != getStaticEntities().end(); it++) {
		if ((*it)->getPosition().y() > entRegion.m_lowerBound) {
			return it;
		}
	}
	return getStaticEntities().begin();
}

EntityList::iterator EntityEngine::getStaticEntitiesRegionEnd(EntityRegion const& entRegion) {
	for (EntityList::iterator it = getStaticEntities().begin(); it != getStaticEntities().end(); it++) {

		if ((*it)->getPosition().y() > entRegion.m_upperBound) {
			return it;
		}
	}
	return getStaticEntities().end();
}

void EntityEngine::cleanBelow(const float yCoord) {
// todo
}

void EntityEngine::removeStaticEntity(EntityList::iterator const& it, Engines & eg) {
// kill the visual

// TODO: memleak, remove this sprite visual in the corresponding unregister function
	(*it)->unregister(eg);
	/*	SpriteVisual * vis = (*it)->getActiveVisual();

	 if (vis != nullptr) {
	 re.removeSpriteVisual(vis);
	 }
	 */
	delete *it;
	m_staticEntities.erase(it);
}

void EntityEngine::removeEntity(EntityList::iterator const& it, Engines & e) {
	(*it)->unregister(e);
	delete *it;
	getEntities().erase(it);
}

void EntityEngine::updatePathfinding() {
	generatePathfindingNodes(m_pathfindingNodes);
}

void EntityEngine::removeEntity(Entity * ent, Engines & e) {
// find the pointer
	auto it = std::find(getEntities().begin(), getEntities().end(), ent);

	if (it == getEntities().end()) {
		logging::Fatal() << "Could not find entity to remove in entity list";
	} else {
		removeEntity(it, e);
	}
}

void EntityEngine::cleanAllStatic(Engines &eg) {
	while (getStaticEntities().begin() != getStaticEntities().end()) {
		removeStaticEntity(getStaticEntities().begin(), eg);
	}
	logging::Info() << "static entity count after kill: " << getStaticEntities().size();
}

void EntityEngine::cleanStaticBelow(const float yCoord, Engines &eg) {
// todo

	typedef std::vector<EntityList::iterator> EntityPtrList;

	EntityPtrList toRemove;

	for (EntityList::iterator it = getStaticEntities().begin(); it != getStaticEntities().end(); it++) {
		if ((*it)->getPosition().y() < yCoord) {
			toRemove.push_back(it);
		}
	}

	for (EntityPtrList::iterator it = toRemove.begin(); it != toRemove.end(); it++) {
		removeStaticEntity(*it, eg);
	}
}

EntityTemplate const& EntityEngine::getTemplate(std::string const& name) const {
	auto it = m_templateMap.find(name);
	if (it == m_templateMap.end()) {
		logging::Fatal() << "Entitiy Template " << name << " not known";
	}

	return *it->second;
}

EntityEngine::TemplateMap const& EntityEngine::getTemplateMap() const {
	return m_templateMap;
}

void EntityEngine::addTemplate(std::string const& name, EntityTemplate * templ) {
	m_templateMap[name] = templ;
}

NodePtr EntityEngine::findClosestNode(Vector2 const& vec) {
	NodePtr minNode = nullptr;
	float minDist = 10000000.0f;

	for (auto & n : m_pathfindingNodes) {
		const float d = n.distanceTo(vec);
		if (d < minDist) {
			minNode = &n;
			minDist = d;
		}
	}

	return minNode;
}
