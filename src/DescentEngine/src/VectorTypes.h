#pragma once

#include <utility>
#include <iostream>
#include <cmath>

#include <bullet/LinearMath/btVector3.h>

typedef btVector3 Vector3;

class Vector2Uint {
public:
	constexpr Vector2Uint() :
			m_x(0.0f), m_y(0.0f) {

	}

	constexpr Vector2Uint(size_t x, size_t y) :
			m_x(x), m_y(y) {

	}

	size_t x() const {
		return m_x;
	}

	size_t y() const {
		return m_y;
	}

private:
	size_t m_x, m_y;
};

class Vector2 {
public:
	constexpr Vector2() :
			m_x(0.0f), m_y(0.0f) {

	}

	constexpr Vector2(float x, float y) :
			m_x(x), m_y(y) {

	}

	static constexpr Vector2 Unit() {
		return Vector2(1.0f, 1.0f);
	}

	static constexpr Vector2 UnitX() {
		return Vector2(1.0f, 0.0f);
	}

	static constexpr Vector2 UnitY() {
		return Vector2(0.0f, 1.0f);
	}

	static constexpr Vector2 Zero() {
		return Vector2(0.0f, 0.0f);
	}

	static float angleBetween(Vector2 const& v1, Vector2 const& v2) {
		auto mul = v1 * v2;
		auto mag = v1.mag() * v2.mag();
		return acos(mul / mag);
	}

	float x() const {
		return m_x;
	}

	float y() const {
		return m_y;
	}

	void setX(const float v) {
		m_x = v;
	}

	void setY(const float v) {
		m_y = v;
	}

	// be careful, as this method only checks for a veeeery small magnitude, but not for exactly zero
	bool nonZero() const {
		return magSquared() > 0.000000001f;
	}

	void alignAlongX() {
		if (x() > 0)
			setX(mag());
		else
			setX(-mag());
		setY(0);
	}

	void alignAlongY() {
		if (y() > 0)
			setY(mag());
		else
			setY(-mag());
		setX(0);
	}

	Vector2 rotatePiHalfClockwiseCopy() const {
		return Vector2(y(), -x());
	}

// todo: optimize the instatiation of all this vector objects here
	Vector2 operator+(Vector2 const& other) const {
		const Vector2 newVec(this->m_x + other.m_x, this->m_y + other.m_y);
		return newVec;
	}

	Vector2 operator-(Vector2 const& other) const {
		const Vector2 newVec(this->m_x - other.m_x, this->m_y - other.m_y);
		return newVec;
	}

	Vector2 operator-() const {
		const Vector2 newVec(-this->m_x, -this->m_y);
		return newVec;
	}

	Vector2 operator*(float factor) const {
		const Vector2 newVec(this->m_x * factor, this->m_y * factor);
		return newVec;
	}

	float operator*(Vector2 const& v) const {
		return this->x() * v.x() + this->y() * v.y();
	}

	Vector2 operator/(float div) const {
		const Vector2 newVec(this->m_x / div, this->m_y / div);
		return newVec;
	}

	Vector2 & operator=(Vector2 const& other) {
		this->m_x = other.x();
		this->m_y = other.y();
		return *this;
	}

	bool operator==(Vector2 const& other) const {
		return (this->m_x == other.x()) && (this->m_y == other.y());
	}

	float magSquared() const {
		return m_x * m_x + m_y * m_y;
	}

	float mag() const {
		return sqrt(magSquared());
	}

	Vector2 normalizedCopy() const {
		const float myMag = this->mag();

		if (myMag == 0.0f) {
			return Zero();
		} else {
			const float norm = 1.0f / myMag;
			const Vector2 newVec(norm * this->x(), norm * this->y());
			return newVec;
		}
	}

// performs a copy of the vector and limits the maximum magnitude
	Vector2 limitedCopy(const float maxMagnitude) const {
		const float myMag = mag();
		float reductionFactor;

		if (myMag > maxMagnitude) {
			reductionFactor = maxMagnitude / myMag;
		} else {
			reductionFactor = 1.0f;
		}
		return (*this) * reductionFactor;
	}

	template<class TListType>
	void addToList(TListType & l) const {
		l.push_back(x());
		l.push_back(y());
	}

private:
	float m_x, m_y;
};

inline std::ostream& operator<<(std::ostream& os, Vector2 const& v) {
	os << "(" << v.x() << "|" << v.y() << ")";
	return os;
}
// first:  offset from origin
// second: direction ( not normalized in all cases )
typedef std::pair<Vector2, Vector2> Line2d;

class VectorSupport {
public:
	static std::pair<bool, Vector2> intersectLines(Line2d a, Line2d b) {
		float numerator = ((b.first.y() - a.first.y()) * a.second.x())
				+ ((a.first.x() - b.first.x()) * a.second.y());
		float denominator = (b.second.x() * a.second.y()) - (b.second.y() * a.second.x());

		if (std::fabs(denominator) < VeeeeerySmall)
			return std::pair<bool, Vector2>(false, Vector2::UnitX());

		float t2 = numerator / denominator;

		Vector2 intersection = b.first + b.second * t2;

		return std::pair<bool, Vector2>(true, intersection);
	}

private:
	static constexpr float VeeeeerySmall = 0.000000000001f;

};

