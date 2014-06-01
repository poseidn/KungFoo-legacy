#pragma once

#include <algorithm>
#include <boost/noncopyable.hpp>

class VisualBase {
public:
	VisualBase() :
			m_visible(true), m_toRemove(false), m_isIngame(true), m_animated(false), m_transparency(1.0f) {
	}

	void setVisible(bool visible) {
		m_visible = visible;
	}

	bool getVisible() const {
		return m_visible;
	}

	void setTransparency(float t) {
		m_transparency = t;
		//std::max((unsigned char) (t * 255.0), (unsigned char) 255);
	}

	float getTransparency() const {
		return m_transparency; //	return m_transparency * 0.00392156862745098f;
	}

	bool toRemove() const {
		return m_toRemove;
	}
	void setToRemove() {
		m_toRemove = true;
	}

	// update the internal buffers of OpenGL
	// in fact, it returns a lambda expression which can be
	// executed by the render engine at the best possible time
	//void update(ScreenTransform const& trans, Vector2 const& pos) = 0;

protected:
	bool m_visible;
	bool m_toRemove;

	bool m_isIngame;
	bool m_animated;

	float m_transparency;
};
