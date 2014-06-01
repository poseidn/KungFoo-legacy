#pragma once

#include <boost/noncopyable.hpp>
#include <list>
#include <memory>

#include "../Cpp11.h"
#include "../Visuals/SpriteVisualList.h"

class EntityAnimationBase;
class ParticleAnimation;
class TextAnimation;

typedef std::shared_ptr<EntityAnimationBase> EntityAnimationBasePtr;

// todo: animation system could also use a integer timing system to be faster and save memory...
class AnimationEngine CPP11_FINAL : boost::noncopyable {
public:
	AnimationEngine();
	~AnimationEngine();

	typedef std::list<EntityAnimationBasePtr> EntityAnimationList;
	typedef std::list<ParticleAnimation *> ParticleAnimationList;
	typedef std::list<TextAnimation *> TextAnimationList;

	void step(const float deltaT);

	void addEntityAnimation(EntityAnimationBasePtr anim);

	void addParticleAnimation(ParticleAnimation * an);

	void addTextAnimation(TextAnimation * an);

	void clearAll();

private:

	EntityAnimationList m_entityAnimations;
	ParticleAnimationList m_particleAnimations;
	TextAnimationList m_textAnimations;

};
