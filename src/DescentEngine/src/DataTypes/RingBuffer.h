#pragma once

#include <array>

template<class TType, size_t TSize>
class RingBuffer {
public:
	RingBuffer() :
			m_currentPosition(0), m_currentSize(0) {
	}

	void push(TType const & t) {
		m_currentPosition++;
		handleCurrentPositionWrapAround();
		m_buffer[m_currentPosition] = t;
		if (m_currentSize < m_buffer.size())
			m_currentSize++;
	}

	// get the most recent entries with get(0)
	// or the ones before with get ( -n )
	// todo: put this in one
	TType & get(unsigned int entry) {
		int itemNumber = m_currentPosition + entry;

		while (itemNumber < 0)
			itemNumber += TSize;

		itemNumber = itemNumber % TSize;
		return m_buffer[itemNumber];
	}

	TType const& get(unsigned int entry) const {
		int itemNumber = m_currentPosition + entry;

		while (itemNumber < 0)
			itemNumber += TSize;

		itemNumber = itemNumber % TSize;
		return m_buffer[itemNumber];
	}

	// size can be < TSize if no items have been added, but is never bigger then
	// TSize
	size_t size() const {
		return m_currentSize;
	}

	int smallestIndex() const {
		// not really true, but the only way to return something useful
		if (size() == 0)
			return 0;

		return -int(size() - 1);
	}

private:

	void handleCurrentPositionWrapAround() {
		if (m_currentPosition >= TSize) {
			m_currentPosition = 0;
		}
	}

	std::array<TType, TSize> m_buffer;
	size_t m_currentPosition;
	size_t m_currentSize;
};

