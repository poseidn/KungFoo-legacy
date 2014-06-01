#pragma once

#include "../Cpp11.h"

#include <LinearMath/btMotionState.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>

class Entity;

//A MotionState is Bullet's way of informing you about updates to an object.
//Pass this MotionState to a btRigidBody to have your SceneNode updated automatically.
class EntityRigidBodyState CPP11_FINAL : public btMotionState {

public:
	EntityRigidBodyState(Entity *ent);

	virtual ~EntityRigidBodyState() {
	}

	Entity * getEntity() const {
		return m_entity;
	}

	virtual void getWorldTransform(btTransform &ret) const CPP11_OVERRIDE;

	virtual void setWorldTransform(const btTransform &in) CPP11_OVERRIDE;

private:
	Entity *m_entity;
};
