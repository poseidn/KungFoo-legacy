#pragma once

#include <map>
#include <tuple>
#include <vector>

#include <boost/noncopyable.hpp>

#include "../Cpp11.h"
#include "../Log.h"
#include "../VectorTypes.h"
#include "CollisionType.h"

class btRigidBody;

class btDiscreteDynamicsWorld;
class btAxisSweep3;
class btSequentialImpulseConstraintSolver;
class btRigidBody;
class btCollisionDispatcher;
class btDefaultCollisionConfiguration;
class btCollisionShape;
struct btDefaultMotionState;

class Entity;

// PhysicsEngine which uses bullet physics as a backend for collision and dynamics
// bullet is "tricked" to move our entities just by the move intents by setting the
// linear velocity before the step simulation to exactly the velocity neede to move to the
// new location
class PhysicsEngine CPP11_FINAL : boost::noncopyable {
public:
	PhysicsEngine();
	~PhysicsEngine();

	void registerEntity(Entity * ent);

	void unregisterEntity(Entity * ent);

	void step(float dt);

	size_t getRegisteredEntitiesCount() const;

private:

	void removeBodyByEntity( Entity * ent );

	struct BodiesEntry {

		BodiesEntry(btRigidBody * gb, short gr, short ma) :
				RigidBody(gb), Group(gr), Mask(ma), LimitedLifetime(false), PhysicsLifetime(0.0f) {

		}

		void setLimitedLifetime(const float ttl) {
			LimitedLifetime = true;
			PhysicsLifetime = ttl;
		}

		btRigidBody * RigidBody;
		// collision group & mask which was used to register this body with the physics system
		// might be changed by external code and the rigid body must be re-registered with bullet
		short Group;
		short Mask;

		// certain enties might only be active in the physics system for a certain time
		// if this is > 0.0f, the lifetime of this item is limited
		bool LimitedLifetime;
		float PhysicsLifetime;
	};

	typedef std::vector<BodiesEntry> BodiesList;

	void unregisterAllEntities();

	void addRigidBody(btRigidBody *, short, short, float);

	BodiesList m_bodies;

	// this objects must be deleted in a certain order
	// therefore no smart pointers used here
	btDiscreteDynamicsWorld* m_phyWorld;
	btAxisSweep3* m_Broadphase;

	btSequentialImpulseConstraintSolver* m_Solver;

	btRigidBody* m_groundRigidBody;

	btCollisionDispatcher* m_Dispatcher;
	btDefaultCollisionConfiguration* m_CollisionConfig;

	// objects needed for the ground shape
	// which is an infinite plane and prevents all physics objects from falling down
	btCollisionShape* m_groundShape;
	btDefaultMotionState* m_groundMotionState;

};

