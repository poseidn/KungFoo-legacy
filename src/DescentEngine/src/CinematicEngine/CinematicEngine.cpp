#include "CinematicEngine.h"

#include "../SoundEngine/SoundEngine.h"
#include "../ResourceEngine/ResourceEngine.h"
#include "../EntityEngine/EntityEngine.h"
#include "../AnimationEngine/AnimationEngine.h"
#include "../AnimationEngine/EntityAnimation.h"
#include "../Engines.h"

void CinematicSound::execute(Engines & eg) {
	auto snd = eg.resourceEngine().loadSound(m_soundName);
	eg.soundEngine().playSound(snd, 0.0f);
}

CinematicTransformAnimation::CinematicTransformAnimation(std::string const& entityName, Vector2 const& start,
		Vector2 final, CinematicTimespan span) :
		CinematicBase(span), m_entityName(entityName), m_start(start), m_final(final) {

}

void CinematicTransformAnimation::execute(Engines & eg)

{
	auto ent = eg.entityEngine().getEntity(m_entityName);
	assert(ent);

	auto an2 = std::make_shared < EntityAnimation<TransformLocation>
			> (*ent, TransformLocation(m_start, m_final, m_span.getDuration()));
	eg.animationEngine().addEntityAnimation(an2);
}

CinematicTranspararencyAnimation::CinematicTranspararencyAnimation(std::string const& entityName, float start,
		float final, CinematicTimespan span) :
		CinematicBase(span), m_entityName(entityName), m_start(start), m_final(final) {

}

void CinematicTranspararencyAnimation::execute(Engines & eg) {
	auto ent = eg.entityEngine().getEntity(m_entityName);
	assert(ent);

	auto an2 = std::make_shared < EntityAnimation<TransformTransparency>
			> (*ent, TransformTransparency(m_start, m_final, m_span.getDuration()));
	eg.animationEngine().addEntityAnimation(an2);
}
