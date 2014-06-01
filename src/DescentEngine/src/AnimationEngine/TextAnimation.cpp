#include "TextAnimation.h"

#include "../Visuals/TextVisual.h"

bool TextAnimation::step(const float deltaT) {

	if (m_offsetBeforeFadeout < 0.0f) {
		m_nextStep -= deltaT;
		const float transparency = std::max(0.0f, m_nextStep / m_stepTime);

		m_visual.setTransparency(transparency);

		if (m_nextStep < 0.0f) {
			// we are sooo DONE !
			// mark to remove
			if (m_removeTextOnAnimationComplete)
				m_visual.setToRemove();
			// cannot use this any more, as the render engine might remove it at any time

			return false;
		}

	} else {
		m_offsetBeforeFadeout -= deltaT;
	}

	return true;
}

