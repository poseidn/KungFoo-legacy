#include "Entity.h"

#include "../Engines.h"

#include "../Physics/PhysicsEngine.h"
#include "../Visuals/SpriteVisual.h"

#include <btBulletDynamicsCommon.h>

void Entity::unregister(Engines & engines) {
	if (getRigidBody() != nullptr) {
		engines.physicsEngine().unregisterEntity(this);
	}
}

Entity::~Entity() {
	if (getRigidBody()) {
		delete getRigidBody()->getMotionState();
		delete getRigidBody()->getCollisionShape();
		delete getRigidBody();
	}
}

VisualUpdatePair Entity::updateVisual(ScreenTransform const& st) {
	auto lmdUpdate = m_activeVisual.get().update(st, getPosition());

	// little hack, run it directly here ...
	// must be done in the SpriteVisual itself to work for statics too
	//lmdUpdate(m_activeVisual);

	return std::make_pair(static_cast<VisualBase *>(nullptr), lmdUpdate);
}
