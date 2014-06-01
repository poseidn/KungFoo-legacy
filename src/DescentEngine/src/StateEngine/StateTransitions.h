#pragma once

#include <tuple>
#include <memory>

#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <map>

#include "../Log.h"
#include "StateEngine.h"

class InputContainer;

// todo: delete states
class StateTransitions: boost::noncopyable {
public:
	//typedef boost::ptr_map<std::string, StateBase> StateList;
	typedef std::map<std::string, StateEngineBase*> StateEngineList;

	StateTransitions() :
			m_activeEngine(nullptr) {

	}

	void step(float delta) {
		ensureActiveState();
		getActiveEngine()->step(delta);

		// state change needed ?
		if (getActiveEngine()->getStateChangeRequest() != "") {
			setActiveEngine(getActiveEngine()->getStateChangeRequest(),
					getActiveEngine()->getStateChangeRequestInfo());
			getActiveEngine()->clearStateChangeRequest();
		}
	}

	void ensureActiveState() {
		assert(m_activeEngine != nullptr);
	}

	void addState(std::string const& stateName, StateEngineBase * ngin) {
		m_stateEngines[stateName] = ngin;

		// enable this one ?
		if (m_activeEngine == nullptr) {
			setActiveEngine(stateName);
		}
	}

	void setActiveEngine(std::string const& stateName, StateChangeInfoPtr changeInfo = nullptr) {
		auto it = m_stateEngines.find(stateName);
		if (it == m_stateEngines.end()) {
			logging::Fatal() << "State engine with name " << stateName << " not loaded";
		}

		if (m_activeEngine != nullptr) {
			m_activeEngine->onDeactivateState();
		}

		m_activeEngine = it->second;

		m_activeEngine->onActivateState(changeInfo);
	}

	StateEngineBase * getActiveEngine() {
		return m_activeEngine;
	}

	bool wantsApplicationQuit() {
		auto ae(getActiveEngine());
		if (ae != nullptr)
			return ae->getApplicationQuitRequest();
		else
			return false;
	}

	std::string getDebugState() const {
		std::stringstream sState;

		for (auto se : m_stateEngines) {
			sState << se.second->getDebugState();
		}

		return sState.str();
	}

private:
	StateEngineList m_stateEngines;

	StateEngineBase * m_activeEngine;

};
