#include "EntityRigidBodyState.h"

#include <bullet/btBulletDynamicsCommon.h>
#include "../EntityEngine/Entity.h"

EntityRigidBodyState::EntityRigidBodyState(Entity *ent) :
		m_entity(ent) {
	assert(m_entity);
}

void EntityRigidBodyState::getWorldTransform(btTransform &ret) const {
	// always ignore rotation
	ret.setRotation(btQuaternion::getIdentity());
	ret.setOrigin(btVector3(m_entity->getPosition().x(), m_entity->getPosition().y(), 5.0f));
}

void EntityRigidBodyState::setWorldTransform(const btTransform &in) {

	const float nearZero = 0.001f;
	// filter out zero movements, othewise the characters will have the run animation all the time
	if ((std::abs(in.getOrigin().x() - m_entity->getPosition().x()) < nearZero)
			&& (std::abs(in.getOrigin().y() - m_entity->getPosition().y()) < nearZero)) {
		return;
	}

	m_entity->setMoveIntent(Vector2(in.getOrigin().x(), in.getOrigin().y()));
}
