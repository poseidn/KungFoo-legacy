#pragma once 

#include <memory>
#include <boost/noncopyable.hpp>

#include "../Util.h"

#include "../VectorTypes.h"
#include "../Rectangle.h"
#include "../TextureRef.h"
#include "../Cpp11.h"
#include "../Visuals/ScreenTransform.h"
#include "../Visuals/DeferredVisualUpdate.h"
#include "../Visuals/SpriteVisualList.h"

//#include "../Visuals/SpriteVisual.h"

#include "../Physics/CollisionType.h"
#include "../Physics/CollisionGroups.h"

//#include "MoveAccessClass.h"
//class MoveAccessClass

// maybe use the Collection SoA here, also to be in shape for
// OpenGL ES Vertex Arrays

// pointer to customs behaviour classes for specialized draw / quantities

class btCollisionObject;
class btRigidBody;

class Engines;

class Entity: private boost::noncopyable {
	friend class MoveAccessClass;

public:
	Entity(Vector2 initialPosition, Rectangle2 boundingBox) :
			m_doesCollisionStick(false), m_position(initialPosition), m_moveIntent(initialPosition), m_boundingBox(
					boundingBox), m_physRigidBody(nullptr), m_positionDirty(false), m_doesCollide(true), m_applyMoveIntent(
					false), m_wasMoved(false), m_collidesInJump(false), m_colGroup(0), m_colMask(
					CollisionGroups::Ground), m_skipCollision(false), m_initialVelocitySet(false) {
	}

	virtual ~Entity();

	// does not take part in the physics simulation as a moving part,
	// only as a collision object
	virtual bool isStatic() const {
		return false;
	}

	bool hasInitialVelocity() const {
		return m_initialVelocity.isValid();
	}

	bool hasLimitedPhysicsLifetime() const {
		return m_limitedPhysicsLifetime.isValid();
	}

	float getLimitedPhysicsLifetime() const {
		return m_limitedPhysicsLifetime.getValue();
	}

	void setLimitedPhysicsLifetime(float ttl) {
		m_limitedPhysicsLifetime = ttl;
	}

	void setInitialVelocity(btVector3 const& v) {
		m_initialVelocity.setValue(v);
	}

	btVector3 retrieveInitialVelocity() {
		btVector3 v = m_initialVelocity.getValue();
		//m_initialVelocity.invalidate();
		return v;
	}

	// can change during runtime
	short collisionGroup() const {
		return m_colGroup;
	}

	// can change during the runtime
	short collisionMask() const {
		return m_colMask;
	}

	void setCollisionGroup(short g) {
		m_colGroup = g;
	}

	// can change during the runtime
	void setCollisionMask(short m) {
		m_colMask = m;
	}

	virtual float getDampingLinear() const {
		return 0.0f;
	}

	virtual float getDampingAngular() const {
		return 0.0f;
	}

	// instructs the physics engine to not compute collision
	// for and with this entity in the following round
	// this can be used when objects must be "beamed" to different
	// places in the game arena ( for example when they died )
	void skipCollsionOneRound() {
		m_skipCollision = true;
	}

	bool isSkipCollsionOneRound() const {
		return m_skipCollision;
	}

	void resetSkipCollsionOneRound() {
		m_skipCollision = false;
	}

	float getCollisionRadius() const {
		return getBoundingBox().getRadius();
	}

	Rectangle2 const& getCollisionBox() const {
		return getBoundingBox();
	}

	virtual CollisionType getCollisionType() const {
		return CollisionType::NoCollideLike;
	}

	bool doesCollisionStick() const {
		return m_doesCollisionStick;
	}

	Vector2 const& getPosition() const {
		return m_position;
	}

	// the new position, where entity intents to be moved after this step
	Vector2 const& getMoveIntent() const {
		return m_moveIntent;
	}

	bool doesCollide() const {
		return m_doesCollide;
	}

	bool doesCollideInJump() const {
		return m_collidesInJump;
	}

	void setDoesCollideInJump(bool v) {
		m_collidesInJump = v;
	}

	void setDoesCollide(bool doesCollide) {
		m_doesCollide = doesCollide;
	}

	Vector2 getMoveDelta() const {
		return getMoveIntent() - getPosition();
	}

	void setMoveDelta(Vector2 const& d) {
		setMoveIntent(getPosition() + d);
	}

	// the entity will not move this step
	void clearMoveDelta() {
		setMoveIntent(getPosition());
	}

	void setMoveIntent(Vector2 const& vec) {
		m_moveIntent = vec;
		m_applyMoveIntent = true;
	}

	Rectangle2 const& getBoundingBox() const {
		return m_boundingBox;
	}

	// todo: cache this
	float getBoundingBoxExtend() const {
		return getBoundingBox().getSize().mag();
	}

	void deltaMoveIntent(Vector2 const& vec) {
		if (vec.nonZero()) {
			const Vector2 newPosition(getMoveIntent() + vec);
			setMoveIntent(newPosition);
		}
	}

	void setActiveVisual(SpriteVisualRef const& visual) {
		m_activeVisual = visual;
		setPositionDirty();
	}

	VisualUpdatePair updateVisual(ScreenTransform const& st);

	SpriteVisualRef getActiveVisual() const {
		return m_activeVisual;
	}

	bool isPositionDirty() const {
		return m_positionDirty;
	}

	void unsetPositionDirty() {
		m_positionDirty = false;
	}

	void setPositionDirty() {
		m_positionDirty = true;
	}

	bool wasMoved() const {
		return m_wasMoved;
	}

	virtual float getMass() const {
		return 1.0f;
	}

	void setRigidBody(btRigidBody * bd) {
		m_physRigidBody = bd;
	}

	btRigidBody * getRigidBody() {
		return m_physRigidBody;
	}

	// gives the implementation of this entity the chance
	// the free any resources which have been allocated with various engines
	// BE SURE TO ALWAYS CALL THIS BASE METHOD IF YOU OVERWRITE
	virtual void unregister(Engines & engines);

	// some protected access to ensure safe multi-threading
	// use the MoveAccessClass to access this stuff
protected:

	void setPosition(Vector2 const& vec) {
		m_positionDirty = true;
		m_position = vec;
		m_moveIntent = vec;
	}

	void applyMoveIntent() {
		m_wasMoved = m_applyMoveIntent;
		if (m_applyMoveIntent) {
			setPosition(getMoveIntent());
			m_applyMoveIntent = false;
		}
	}

	bool m_doesCollisionStick;

private:
	// centered in the bounding box
	Vector2 m_position;
	Vector2 m_moveIntent;

	Rectangle2 m_boundingBox;
	Rectangle2 m_paintBox;

	SpriteVisualRef m_activeVisual;
	btRigidBody * m_physRigidBody;

	// has the position or size been changed ? if so, recompute the vertex buffer
	// in the next round
	// TODO: can be done asynchronous / threaded
	bool m_positionDirty;
	bool m_doesCollide;
	bool m_applyMoveIntent;
	bool m_wasMoved;

	bool m_collidesInJump;
	short m_colGroup;
	short m_colMask;
	bool m_skipCollision;

	util::ValidValue<btVector3> m_initialVelocity;
	util::ValidValue<float> m_limitedPhysicsLifetime;
public:
	bool m_initialVelocitySet;
};

