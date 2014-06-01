#pragma once

#include <iomanip>
#include <map>
#include <boost/noncopyable.hpp>
#include <chrono>

#include "../Log.h"

#ifdef DESCENT_PROFILE
// todo: average of the last n measurements
class SectionRepo: boost::noncopyable {
public:
	typedef std::chrono::microseconds Duration;

	void storeResult(std::string const& name, Duration dr) {
		m_durations[name] = dr;
	}

	void report() {
		size_t overallMs = 0;
		for (auto const& it : m_durations) {
			overallMs += it.second.count();
		}

		for (auto const& it : m_durations) {
			const float ratioOfAll = float(it.second.count()) / float(overallMs);
			logging::Info() << std::setprecision(2) << std::endl << "Comp: " << it.first << "\t\t" << ratioOfAll
			<< " :: " << it.second.count();
		}

		logging::Info() << std::endl << "Overtime time (mu sec) " << overallMs << " fps: "
		<< 1.0f / (float(overallMs) / 1000000.0f) << std::endl;
		logging::Info() << std::endl;
	}

private:
	std::map<std::string, Duration> m_durations;
};

class SectionTimer: boost::noncopyable {
public:
	SectionTimer(SectionRepo & rep, std::string const& sectionName) :
	m_repo(rep), m_sectionName(sectionName) {
		m_startTime = std::chrono::high_resolution_clock::now();
	}

	~SectionTimer() {
		auto endTime = std::chrono::high_resolution_clock::now();
		m_repo.storeResult(m_sectionName,
				std::chrono::duration_cast < std::chrono::microseconds > (endTime - m_startTime));
	}

private:
	SectionRepo & m_repo;
	std::string m_sectionName;
	std::chrono::high_resolution_clock::time_point m_startTime;

};

#else

class SectionRepo: boost::noncopyable {
public:
	void report() {
	}
};

class SectionTimer: boost::noncopyable {
public:
	SectionTimer(SectionRepo & rep, std::string const& sectionName) {
	}
};

#endif

class GlobalTimingRepo {
public:
	static SectionRepo Rep;
};
