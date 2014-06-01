#pragma once

#include "VectorTypes.h"

// Careful: There is a API call on windows which confuses the compiler, that why
// this is called Rectangl2
class Rectangle2 {
public:
	Rectangle2() {

	}

	Rectangle2(float x, float y) :
			m_upperLeft(0.0f, 0.0f), m_lowerRight(x, y) {

	}

	Rectangle2(float x0, float y0, float x1, float y1) :
			m_upperLeft(x0, y0), m_lowerRight(x1, y1) {

	}

	Rectangle2(Rectangle2 const& r) {
		m_upperLeft = r.getUpperLeft();
		m_lowerRight = r.getLowerRight();
	}

	const Vector2& getUpperLeft() const {
		return m_upperLeft;
	}

	const Vector2& getLowerRight() const {
		return m_lowerRight;
	}

	const Vector2 getUpperLeftGlobal(Vector2 const& globalCenter) const {
		return Vector2(globalCenter.x() - getSize().x() * 0.5f, globalCenter.y() + getSize().y() * 0.5f);
	}

	const Vector2 getUpperRightGlobal(Vector2 const& globalCenter) const {
		return Vector2(globalCenter.x() + getSize().x() * 0.5f, globalCenter.y() + getSize().y() * 0.5f);
	}

	const Vector2 getLowerRightGlobal(Vector2 const& globalCenter) const {
		return Vector2(globalCenter.x() + getSize().x() * 0.5f, globalCenter.y() - getSize().y() * 0.5f);
	}

	const Vector2 getLowerLeftGlobal(Vector2 const& globalCenter) const {
		return Vector2(globalCenter.x() - getSize().x() * 0.5f, globalCenter.y() - getSize().y() * 0.5f);
	}

	float width() const {
		return getSize().x();
	}

	float height() const {
		return getSize().y();
	}

	float getRadius() const {
		return getSize().mag() * 0.5f;
	}

	const Vector2 getSize() const {
		return Vector2(m_lowerRight.x() - m_upperLeft.x(), m_lowerRight.y() - m_upperLeft.y());
	}

	static Rectangle2 Zero() {
		return Rectangle2(0, 0, 0, 0);
	}

	static Rectangle2 Unit() {
		return Rectangle2(0, 0, 1, 1);
	}

private:
	Vector2 m_upperLeft;
	Vector2 m_lowerRight;
};

