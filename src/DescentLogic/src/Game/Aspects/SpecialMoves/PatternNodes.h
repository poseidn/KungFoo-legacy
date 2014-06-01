#pragma once

#include "InputItem.h"
#include "Deactivator.h"

namespace special_move {

class PatternNodeBase {
public:
	// match found, position of the first matched element, position of the last matched element
	typedef std::tuple<bool, int, int> MatchResult;

	virtual ~PatternNodeBase() {
	}

	virtual MatchResult match(InputRecordType const&, Deactivator &, int, int) const= 0;

};

class SingleNode: public PatternNodeBase {
public:
	SingleNode(InputItem inp) :
			m_inp(inp) {
	}
	virtual ~SingleNode() {
	}

	// going from the newest to the oldest entries
	virtual MatchResult match(InputRecordType const& record, Deactivator & deac, int startPosition = 1,
			int endPosition = 1) const CPP11_OVERRIDE {
		if (startPosition > 0)
		startPosition = 0;
		if (endPosition > 0)
		endPosition = record.smallestIndex();

		for (int i = startPosition; i >= endPosition; i--) {
			InputItem const& it = record.get(i);

			if ((m_inp == it) && (!it.Used)) {
				// did match, mark as used
				deac.registerIndex(i);
				return std::make_tuple(true, i, i);
			}
		}
		return std::make_tuple(false, 0, 0);
	}

private:
	const InputItem m_inp;
};

// both must appear in certain time window
class ConjuctionNode: public PatternNodeBase {
public:
	ConjuctionNode(InputItem a, InputItem b, float maxTimeDistance = -1.0f) :
			m_nodeA(SingleNode(a)), m_nodeB(SingleNode(b)), m_maxTimeDistance(maxTimeDistance) {

	}

	virtual ~ConjuctionNode() {
	}

	virtual MatchResult match(InputRecordType const& record, Deactivator & deac, int startPosition = 1,
			int endPosition = 1) const CPP11_OVERRIDE {
		// run both and compine their result
		const MatchResult resA = m_nodeA.match(record, deac, startPosition, endPosition);
		const MatchResult resB = m_nodeB.match(record, deac, startPosition, endPosition);

		const int posMatchA = std::get < 1 > (resA);
		const int posMatchB = std::get < 1 > (resB);

		const int beginItem = std::max(posMatchA, posMatchB);
		const int endItem = std::min(posMatchA, posMatchB);

		const bool bothMatched = std::get < 0 > (resA) && std::get < 0 > (resB);

		if (bothMatched) {
			if (m_maxTimeDistance > 0.0f) {
				const float dur = computeTimeDistance(record, beginItem,
						endItem);
				if (dur > m_maxTimeDistance)
				return std::make_tuple(false, 0, 0);
			}
		}

		return std::make_tuple(bothMatched, beginItem, endItem);
	}

private:
	const SingleNode m_nodeA;
	const SingleNode m_nodeB;
	const float m_maxTimeDistance;
};

}
