#pragma once

class FloatCharZeroOne {
public:

	float get() const {
		return float(m_ch) * fromFact;
	}

	void set(float f) {
		m_ch = char(std::min(f * toFact, 255.0f));
	}

private:
	constexpr static float maxFloatVal = 1.0f;
	constexpr static float maxCharVal = 255.0f;

	constexpr static float toFact = maxFloatVal / maxCharVal;
	constexpr static float fromFact = 1.0f / toFact;

	unsigned char m_ch;
};

