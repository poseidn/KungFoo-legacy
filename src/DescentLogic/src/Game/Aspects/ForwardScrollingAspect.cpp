#include "ForwardScrollingAspect.h"

#include "../GameState.h"
#include "../../Entities/PlayerEntity.h"
#include "../../Config/GameMovement.h"

#include "../../LevelFactory.h"

void ForwardScrollingAspect::init(GameState & gs) {
	gs.slotStep.subscribe([this] ( GameState & g, float t)
	{	this->step(g,t);});

	gs.slotDebugSwitchScrolling.subscribe([this ]( GameState & g, bool enabled)
	{	this->switchScrolling(g);});
}

void ForwardScrollingAspect::step(GameState & gs, float deltaT) {
	if (m_scrollEnabled) {
		const float streamingBufferSize = 20.0f;

		Vector3 vec = gs.getEngines().renderEngine().getCameraLocation();
		vec.setY(vec.y() + deltaT * GameMovement::ScrollSpeed);
		gs.getEngines().renderEngine().setCameraLocation(vec);

// stream new level content
		//if ((vec.y() - 10.0) > m_locationBuffer) {
		//	logging::Debug<>() << "Streaming the next layers";

		// will automatically detect if we need a new level fragment
		gs.getLevelFactory()->nextLayers(gs, vec.y());
//			m_locationBuffer = vec.y();
//		}

		// remove old stuff
		gs.getEngines().entityEngine().cleanStaticBelow(vec.y() - streamingBufferSize, gs.getEngines());
	}
// clean-up old entities
//gs.getEngines().entityEngine().cleanBelow ( gs.getPlayerEntity()->getPosition().y - 20.0f );
}

void ForwardScrollingAspect::switchScrolling(GameState & gs) {
	m_scrollEnabled = !m_scrollEnabled;
}

