#pragma once

#include <cmath>
#include <map>

#include "VectorTypes.h"

namespace util {

template<class TVal>
class ValidValue {
public:
	ValidValue() :
			m_isValid(false) {
	}

	void setValue(TVal const& v) {
		m_value = v;
		m_isValid = true;
	}

	TVal const& getValue() const {
		// one should never ask for an invalid value
		assert(isValid());
		return m_value;
	}

	void invalidate() {
		m_isValid = false;
	}

	bool isValid() const {
		return m_isValid;
	}

	ValidValue<TVal> & operator=(TVal const& other) {
		setValue(other);
		// by convention, always return *this
		return *this;
	}

private:
	bool m_isValid;
	TVal m_value;
};

/* class to run a certain computation only once and always return the result
 *
 * 		auto lmbDirection = [&pent, &gs] () -> float {
 constexpr float maxDirection = 0.9f;

 // compute player location
 float direction = pent->getPosition().x()
 / gs.getEngines().renderEngine().getScreenTransform().screenSizeInTiles().x();
 // shift to -0.7 to 0.7
 direction = (direction * maxDirection * 2.0f) - maxDirection;
 return direction;
 };

 util::RunOnce<decltype( lmbDirection), float> direction(lmbDirection);
 */

// todo: get the return type from the std::function object
template<class TLambda, class TResult>
class RunOnce {
public:
	RunOnce(TLambda const lmb) :
			m_lmb(lmb), m_res(), m_wasRun(false) {

	}

	TResult result() {
		if (m_wasRun)
			return m_res;

		m_res = m_lmb();
		m_wasRun = true;

		return m_res;
	}

private:
	TLambda m_lmb;
	TResult m_res;
	bool m_wasRun;
};

template<class TKey, class TVal>
inline bool initMap(std::map<TKey, TVal> & m, TKey const& k) {
	auto it = m.find(k);
	if (it == m.end()) {
		// add it !
		m[k] = TVal();
		return true;
	}

	return false;
}

template<class TFloatType>
inline bool withinDelta(TFloatType const& valMust, TFloatType const& val, TFloatType const& delta) {
	return (std::abs(valMust - val) < delta);
}

inline bool withinDelta(Vector2 const& valMust, Vector2 const& val, float const& deltaSquared) {
	const float dist = (valMust - val).magSquared();
	return dist < deltaSquared;
}

constexpr inline float sq(float f1) {
	return f1 * f1;
}

}

