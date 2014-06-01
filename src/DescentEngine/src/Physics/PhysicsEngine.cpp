#include "PhysicsEngine.h"

#include <algorithm>

#include "../Util.h"

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/btBulletCollisionCommon.h>

#include "../EntityEngine/Entity.h"

#include "CollisionGroups.h"
#include "EntityRigidBodyState.h"

PhysicsEngine::PhysicsEngine() {
	m_Broadphase = new btAxisSweep3(btVector3(-100, -100, -100), btVector3(100, 1000000, 100));
	m_CollisionConfig = new btDefaultCollisionConfiguration();
	m_Dispatcher = new btCollisionDispatcher(m_CollisionConfig);

	m_Solver = new btSequentialImpulseConstraintSolver();
	m_phyWorld = new btDiscreteDynamicsWorld(m_Dispatcher, m_Broadphase, m_Solver, m_CollisionConfig);

	//m_phyWorld->setGravity(btVector3(0, 0, -9.8f));
	m_phyWorld->setGravity(btVector3(0, 0, 0));

	// create shape for zero plane
	/*	m_groundShape = new btStaticPlaneShape(btVector3(0, 0, 1), 1);

	 m_groundMotionState = new btDefaultMotionState(
	 btTransform(btQuaternion::getIdentity(), btVector3(0, -1, 0)));
	 btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, m_groundMotionState, m_groundShape,
	 btVector3(0, 0, 0));
	 m_groundRigidBody = new btRigidBody(groundRigidBodyCI);

	 m_phyWorld->addRigidBody(m_groundRigidBody, CollisionGroups::Ground,
	 0 // will itself collide with nothing
	 );
	 */
}

PhysicsEngine::~PhysicsEngine() {

	unregisterAllEntities();

	//m_phyWorld->removeRigidBody(m_groundRigidBody);

	//delete m_groundMotionState;
	//delete m_groundRigidBody;
	//delete m_groundShape;

	// order is important here, otherwise there will be segfaults
	// taken from: BasicDemo.cpp
	delete m_phyWorld;
	delete m_Solver;
	delete m_Broadphase;
	delete m_Dispatcher;
	delete m_CollisionConfig;
}

void PhysicsEngine::unregisterAllEntities() {
	for (auto & bd : m_bodies) {
		m_phyWorld->removeRigidBody(bd.RigidBody);
		// don't delete here, this will be done in the entities
		// dtor
	}
	m_bodies.clear();
}

void PhysicsEngine::removeBodyByEntity(Entity * ent) {

	btRigidBody * thisRb = ent->getRigidBody();
	assert(thisRb);

	const size_t prevSize = m_bodies.size();
	// does not actually remove, but returns an iterator to the first element removed
	auto itRem = std::remove_if(m_bodies.begin(), m_bodies.end(),
			[thisRb] ( BodiesEntry & be ) {return be.RigidBody == thisRb;});
	//logging::Info() << " is end " << (itRem == m_bodies.end());
	m_bodies.erase(itRem);
	assert((prevSize - 1) == m_bodies.size());

	/*	if ((prevSize - 1) == m_bodies.size()) {
	 logging::Info() << "## REMOVED " << prevSize;
	 } else {
	 logging::Info() << "## NOT REMOVED " << prevSize << " : " << m_bodies.size();
	 assert(false);
	 }*/

}

void PhysicsEngine::unregisterEntity(Entity * ent) {

	btRigidBody * thisRb = ent->getRigidBody();
	assert(thisRb);
	m_phyWorld->removeRigidBody(thisRb);

	removeBodyByEntity(ent);

}

void PhysicsEngine::registerEntity(Entity * ent) {
// everything is 0.8 high and the plane is at z = 0.0

	const float wallHeight = 9.5f;

	if (ent->getCollisionType() == CollisionType::NoCollideLike)
		return;

	btCollisionShape * shp = nullptr;

	float mass = ent->getMass();
	bool isDynamic = true;

	if (ent->getCollisionType() == CollisionType::BoxLike) {
		const Rectangle2 rect = ent->getCollisionBox();

		// important, so boxes will not be affectey by dynamics
		isDynamic = false;
		mass = 0.0f;
		shp = new btBoxShape(btVector3(rect.width() * 0.5, rect.height() * 0.5, wallHeight));
		//std::cout << "width " << rect.width() * 0.5 << " height " << rect.height() * 0.5;
	} else if (ent->getCollisionType() == CollisionType::CircleLike) {
		//std::cout << "Adding circellike" << std::endl;
		shp = new btSphereShape(ent->getCollisionRadius());
	}
	assert(shp != nullptr);

//Create BtOgre MotionState (connects Ogre and Bullet).
	EntityRigidBodyState *btState = new EntityRigidBodyState(ent);

//Create the Body.
	btRigidBody * bulletBody = new btRigidBody(mass, btState, shp);
//bulletBody->setDamping(ent->getDampingLinear(), ent->getDampingAngular());

	if (isDynamic) {
		bulletBody->forceActivationState(DISABLE_DEACTIVATION);
	} else {
		bulletBody->setCollisionFlags(
				bulletBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		bulletBody->setActivationState(DISABLE_DEACTIVATION);
	}

	ent->setRigidBody(bulletBody);

// below zero means no limitation
	float limitedLifetime = -1.0f;
	if (ent->hasLimitedPhysicsLifetime()) {
		limitedLifetime = ent->getLimitedPhysicsLifetime();
	}

	addRigidBody(bulletBody, ent->collisionGroup(), ent->collisionMask(), limitedLifetime);
}

void PhysicsEngine::step(float dt) {

	std::vector<Entity *> toRemove;

	// set the velocities for all entities to reach their target in this time step
	for (auto & bd : m_bodies) {
		btMotionState * motState = bd.RigidBody->getMotionState();
		EntityRigidBodyState * entState = static_cast<EntityRigidBodyState *>(motState);
		// never need to move statics
		Entity * ent = entState->getEntity();

		// has this entity to be disabled ?
		if (bd.LimitedLifetime && !ent->isStatic()) {
			bd.PhysicsLifetime -= dt;
			if (bd.PhysicsLifetime < 0.0f) {
				// lifetime is over, remove from simulation as soon as possible
				toRemove.push_back(ent);
				continue;
			}
		}

		if (ent->isStatic())
			continue;

		if (ent->hasInitialVelocity()) {
			if (!ent->m_initialVelocitySet) {
				bd.RigidBody->setLinearVelocity(ent->retrieveInitialVelocity());
				ent->m_initialVelocitySet = false;
			}

			continue;
		}

		const Vector2 con = ent->getMoveDelta();
		const float speed = con.mag() / dt;
		float speedZ = 0.0f;

		// ensure all moving rigid bodies (spheres) stay on the same z plane ( 5.0 )
		// after some gametime, they might be shifting slowing to upper / lower z plane
		if (!util::withinDelta(5.0f, bd.RigidBody->getCenterOfMassPosition().z(), 0.1f)) {
			const float moveZ = -(bd.RigidBody->getCenterOfMassPosition().z() - 5.0f);
			speedZ = moveZ / dt;
		}

		if (speed > 0.01f) {
			const Vector2 dir = con.normalizedCopy();

			bd.RigidBody->setLinearVelocity(btVector3(dir.x() * speed, dir.y() * speed, speedZ));
		} else {
			bd.RigidBody->setLinearVelocity(btVector3(0.0f, 0.0f, speedZ));
		}

		// did group or mask change so we need to re-add ?
		if ((ent->collisionGroup() != bd.Group) || (ent->collisionMask() != bd.Mask)
				|| ent->isSkipCollsionOneRound()) {
			// re-add ...
			m_phyWorld->removeRigidBody(bd.RigidBody);

			if (ent->isSkipCollsionOneRound()) {
				// will be automatically fixed in the next simulation step,
				// as the Physics Bodies and the Entities Group/Mask don't math
				bd.Group = 0;
				bd.Mask = 0;

				ent->resetSkipCollsionOneRound();
			} else {
				bd.Group = ent->collisionGroup();
				bd.Mask = ent->collisionMask();
			}

			m_phyWorld->addRigidBody(bd.RigidBody, bd.Group, bd.Mask);
		}

	}

	// are there any bodies to remove ?
	for (Entity * ent : toRemove) {
		unregisterEntity(ent);
	}

	m_phyWorld->stepSimulation(dt, 10);
}

size_t PhysicsEngine::getRegisteredEntitiesCount() const {
	return m_bodies.size();
}

void PhysicsEngine::addRigidBody(btRigidBody * rb, short group, short mask, float limitedLifetime) {
	m_bodies.push_back(BodiesEntry(rb, group, mask));
	if (limitedLifetime > 0.0f) {
		m_bodies.back().setLimitedLifetime(limitedLifetime);
	}

	m_phyWorld->addRigidBody(rb, group, mask);
}
