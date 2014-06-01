#pragma once

#include <DescentEngine/src/Cpp11.h>
#include "FighterEntity.h"

class EnemyEntity CPP11_FINAL : public FighterEntity {
public:
	EnemyEntity(Vector2 intialPosition, Rectangle2 boundingBox) :
			FighterEntity(intialPosition, boundingBox), m_isAttacking(false) {
	}

	virtual ~EnemyEntity() {
	}

	void startAttack() {
		m_isAttacking = true;
	}

	void stopAttack() {
		m_isAttacking = false;
	}

	bool isAttacking() const {
		return m_isAttacking;
	}

private:
	bool m_isAttacking;
};
