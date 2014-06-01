#pragma once

#include <random>
#include <map>
#include <array>
#include <tuple>
#include <vector>
#include <stack>
#include <functional>

#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/math/constants/constants.hpp>
#include <DescentEngine/src/Cpp11.h>

#include <DescentEngine/src/VectorTypes.h>
#include <DescentEngine/src/StateEngine/Aspect.h>
#include <DescentEngine/src/DataTypes/RingBuffer.h>

#include "../GameState.h"

class ComboDetectionAspect CPP11_FINAL : public Aspect<GameState> {
public:

	ComboDetectionAspect() {
	}

	virtual ~ComboDetectionAspect() {
	}

// holds an input pattern that will be compared to the player input

	virtual void init(GameState & gs) CPP11_OVERRIDE;

private:
	class Kill {
	public:
		Kill() :
				UsedForCombo(0) {
		}

		Kill(float ds) :
				Timestamp(ds), UsedForCombo(0) {
		}
		float Timestamp;
		int UsedForCombo;

	};

	static constexpr int Used_5_Single_BitField = 1 << 0;
	static constexpr int Used_10_Single_BitField = 1 << 1;
	static constexpr int Used_10_Multi_BitField = 1 << 2;

	static constexpr size_t RingBufferSize = 15;
	typedef RingBuffer<Kill, RingBufferSize> KillBuffer;

	class PlayerKills {
	public:
		KillBuffer Kills;
	};

	std::map<PlayerId, PlayerKills> m_kills;

	void analyze(GameState & gs);
	bool bufferEntriesUnused(KillBuffer const& buffer, int bit, int start, int end);
	void markEntriesUsed(KillBuffer & buffer, int bit, int start, int end);

};

