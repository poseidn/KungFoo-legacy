#include "AnimationEngine.h"

#include <vector>

#include "EntityAnimation.h"
#include "ParticleAnimation.h"
#include "TextAnimation.h"

AnimationEngine::AnimationEngine() {
}

AnimationEngine::~AnimationEngine() {
}

void AnimationEngine::clearAll() {
	while (m_particleAnimations.size() != 0) {
		auto it = m_particleAnimations.begin();
		delete (*it);
		m_particleAnimations.erase(it);
	}

	while (m_textAnimations.size() != 0) {
		auto it = m_textAnimations.begin();
		delete (*it);
		m_textAnimations.erase(it);
	}

	m_entityAnimations.clear();
}

void AnimationEngine::step(const float deltaT) {

	std::vector < TextAnimationList::iterator > textKillList;

	// text
	for (TextAnimationList::iterator it = m_textAnimations.begin(); it != m_textAnimations.end(); it++) {
		if ((*it)->step(deltaT) == false) {
			textKillList.push_back(it);
		}
	}
	for (auto & it : textKillList) {
		delete (*it);
		m_textAnimations.erase(it);
	}

	// particles
	std::vector < ParticleAnimationList::iterator > particleKillList;

	for (ParticleAnimationList::iterator it = m_particleAnimations.begin(); it != m_particleAnimations.end();
			it++) {
		if ((*it)->step(deltaT) == false) {
			particleKillList.push_back(it);
		}
	}

	for (auto & it : particleKillList) {
		delete (*it);
		m_particleAnimations.erase(it);
	}

	// entity animations
	std::vector < EntityAnimationList::iterator > killList;

	for (EntityAnimationList::iterator it = m_entityAnimations.begin(); it != m_entityAnimations.end(); it++) {

		if ((*it)->decreaseDuration(deltaT)) {
			// animation still running
			(*it)->step(deltaT);
		} else {
			// animation is done and needs to be killed
			killList.push_back(it);
		}
	}

	for (auto & it : killList) {
		m_entityAnimations.erase(it);
	}

}

void AnimationEngine::addEntityAnimation(EntityAnimationBasePtr anim) {
	m_entityAnimations.push_back(anim);
}

void AnimationEngine::addParticleAnimation(ParticleAnimation * an) {
	m_particleAnimations.push_back(an);
}

void AnimationEngine::addTextAnimation(TextAnimation * an) {
	m_textAnimations.push_back(an);
}

