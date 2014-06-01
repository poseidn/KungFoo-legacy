#pragma once

#include <cmath>

#include <DescentEngine/src/EntityEngine/MultiVisualEntity.h>
#include <DescentEngine/src/EntityEngine/TimeSliceAction.h>
#include <DescentEngine/src/Util.h>
#include <DescentLogic/src/Config/EnemyBehaviour.h>


#include "GameCollisionGroups.h"
#include "TextureIds.h"

class FighterEntity: public MultiVisualEntity {
public:
	FighterEntity(Vector2 intialPosition, Rectangle2 boundingBox);

	virtual ~FighterEntity() {
	}

	virtual CollisionType getCollisionType() const CPP11_OVERRIDE {
		return CollisionType::CircleLike;
	}

	TimeSliceAction & getJumpAction() {
		return m_jumpAction;
	}

	TimeSliceAction & getHitAction() {
		return m_hitAction;
	}

	TimeSliceAction & getKickAction() {
		return m_kickAction;
	}

	bool decreaseLastHit(float dt) {
		if (m_lastHit < 0.0f)
			return true;

		m_lastHit -= dt;
		if (m_lastHit < 0.0f)
			return true;
		else
			return false;
	}

	void activateLastHit() {
		m_lastHit = EnemyBehaviour::RegenerationAfterHit;
	}

	virtual void validateActions(float deltaT, float hitRegenTime);

	void subtractLife(int amount) {
		m_life -= amount;
	}

	int getLife() const {
		return m_life;
	}

	bool isDead() const {
		return m_dead;
	}

	void die();

	void setDirection(Vector2 const d) {
		m_direction = d.normalizedCopy();
	}

	bool isLyingFlat() const {
		//return m_hitAction.isActive();
		return m_dead;
	}

	// let's this entity run to a position as fast as possible
	float runTo(Vector2 location, float maxSpeed, float deltaT, EntityEngine & entNG, float closeThreshold =
			0.0f);

	Vector2
	const& getDirection() const {
		return m_direction;
	}

	float getAttackTime() const {
		return m_attackTime;
	}

	void decreaseAttackTime(float by) {
		m_attackTime -= by;
	}

	virtual void updateVisual(Engines & eg);

	int getCurrentSekktor() const;

private:
	TimeSliceAction m_hitAction;

	TimeSliceAction m_jumpAction;
	TimeSliceAction m_kickAction;
	Vector2 m_direction;
	bool m_dead;
	int m_life;
	float m_attackTime;
	float m_lastHit;

	// the next node to move to, which was found by the path finding
	util::ValidValue<Vector2> m_nextNode;
}
;
