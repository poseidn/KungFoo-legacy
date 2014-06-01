#pragma once

#include <pthread.h>
#include <functional>
#include <memory>
#include <thread>
#include <mutex>

#include <boost/noncopyable.hpp>

#include "Cpp11.h"

#include "Threading/PortableMutex.h"
#include "Threading/PortableThread.h"

// register for events with a template system ?

#include "Engines.h"

class StateTransitions;

// todo: decrease usage of templates here
class GameLoop CPP11_FINAL : boost::noncopyable {
public:
	static constexpr bool UseThreading = false;

	GameLoop(RenderEngine & re, StateTransitions & trans, EntityEngine& ee, AnimationEngine & an,
			InputSystem & input, PhysicsEngine & phys, bool showFps) :
			m_renderEngine(re), //
			m_stateTransitions(trans), //
			m_entityEngine(ee), //
			m_animationEngine(an), //
			m_inputSystem(input), //
			m_physics(phys), //
			m_maxFramerate(100.0f), //
			m_showFps(showFps), //
			m_loopIteration(0) {
	}

	/// can be used in an application which does not want to return control
	void run();
	// timeDelta seconds
	void step(float timeDelta);

	void setInitialContentFunc(std::function<void(void)> f);

	void setRenderInitData(RenderEngine::interface_type::InterfaceDataType const& co) {
		m_initData = co;
	}

private:

	// is only run, if not multi-threaded
	void render();
	void initRender();

	RenderEngine & m_renderEngine;
	StateTransitions & m_stateTransitions;
	EntityEngine & m_entityEngine;
	AnimationEngine & m_animationEngine;
	InputSystem & m_inputSystem;
	PhysicsEngine &m_physics;

	std::mutex m_mutxVertexData;
	//bool m_renderInitialized;
	std::function<void(void)> m_initialContentFunc;
	//std::unique_ptr<PortableThread> m_renderThread;

	RenderEngine::interface_type::InterfaceDataType m_initData;

	// in frames / second
	const float m_maxFramerate;
	const bool m_showFps;
	size_t m_loopIteration;
};
