#pragma once

#if defined(STUCK_MSVC_COMPILE)
#include <Windows.h>
#endif

#ifdef USE_ANDROID
#include <android/log.h>
#define ANDROID_LOGI( cstr ) ((void)__android_log_print(ANDROID_LOG_INFO, "FastDescent", "%s", cstr ))
#define ANDROID_LOGW( cstr ) ((void)__android_log_print(ANDROID_LOG_WARN, "FastDescent", "%s", cstr ))
#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>

namespace logging {
//void message( std::string const& s)
//{
//_internal_log("Init Bullet Physics ...");

namespace MessageLevel {
enum MessageLevelEnum {
	Debug, Normal, Critical
};

}

#define DESCENT_LOG_FILENAME "descent.log"

inline void _internal_log(std::string message, MessageLevel::MessageLevelEnum lvl) {
#ifdef USE_ANDROID

	if ( lvl == MessageLevel::Critical ) {
		ANDROID_LOGW ( message.c_str());
	}
	else {
		ANDROID_LOGI ( message.c_str());
	}
#else
	std::cout << message << std::endl;

	std::ofstream outFile(DESCENT_LOG_FILENAME, std::ios::app);
	outFile << message << std::endl;
	outFile.close();
#endif
}

inline void resetLogFile() {
	// will empty the file
	std::ofstream outFile(DESCENT_LOG_FILENAME, std::ios::trunc);
	outFile.close();
}

namespace Categories {
class Collision {
};
class GameLoop {
};
class RenderEngine {
};
class Default {
};
}

class Info {
public:
	explicit Info() {
	}

// called once the chain of << operators is done...
	~Info() {
		_internal_log(m_stream.str(), MessageLevel::Normal);
	}

	template<class T>
	Info &
	operator<<(T const & t) {
		m_stream << t;
		return *this;
	}
	Info &
	operator<<(std::ostream&(*f)(std::ostream&)) {
		m_stream << f;
		return *this;
	}
	Info &
	operator<<(std::ios_base&(*f)(std::ios_base&)) {
		m_stream << f;
		return *this;
	}
private:
//MessageSender ap;
	Info(Info const&); // Change log 9
	Info& operator=(Info const&);
	std::stringstream m_stream;
};

template<class TLogCategory = Categories::Default>
class Debug {
public:
	explicit Debug() {
	}

// called once the chain of << operators is done...
	~Debug() {
		_internal_log(m_stream.str(), MessageLevel::Debug);
	}

	template<class T>
	Debug &
	operator<<(T const & t) {
		m_stream << t;
		return *this;
	}
	Debug &
	operator<<(std::ostream&(*f)(std::ostream&)) {
		m_stream << f;
		return *this;
	}
	Debug &
	operator<<(std::ios_base&(*f)(std::ios_base&)) {
		m_stream << f;
		return *this;
	}
private:
//MessageSender ap;
	Debug(Debug const&); // Change log 9
	Debug& operator=(Debug const&);
	std::stringstream m_stream;

};

template<>
class Debug<Categories::Collision> {
public:
	explicit Debug() {
	}
	~Debug() {

	}

	template<class T>
	Debug &
	operator<<(T const & t) {

		return *this;
	}
	Debug &
	operator<<(std::ostream&(*f)(std::ostream&)) {

		return *this;
	}
	Debug &
	operator<<(std::ios_base&(*f)(std::ios_base&)) {
		return *this;
	}
private:
	Debug(Debug const&); // Change log 9
	Debug& operator=(Debug const&);
};

template<>
class Debug<Categories::GameLoop> {
public:
	explicit Debug() {
	}
	~Debug() {

	}

	template<class T>
	Debug &
	operator<<(T const & t) {

		return *this;
	}
	Debug &
	operator<<(std::ostream&(*f)(std::ostream&)) {

		return *this;
	}
	Debug &
	operator<<(std::ios_base&(*f)(std::ios_base&)) {
		return *this;
	}
private:
	Debug(Debug const&); // Change log 9
	Debug& operator=(Debug const&);
};

template<>
class Debug<Categories::RenderEngine> {
public:
	explicit Debug() {
	}
	~Debug() {

	}

	template<class T>
	Debug &
	operator<<(T const & t) {

		return *this;
	}
	Debug &
	operator<<(std::ostream&(*f)(std::ostream&)) {

		return *this;
	}
	Debug &
	operator<<(std::ios_base&(*f)(std::ios_base&)) {
		return *this;
	}
private:
	Debug(Debug const&); // Change log 9
	Debug& operator=(Debug const&);
};

class Error {
public:
	explicit Error() {
		m_stream << "ERROR: ";
	}

// called once the chain of << operators is done...
	~Error() {
		_internal_log(m_stream.str(), MessageLevel::Critical);
	}

	template<class T>
	Error &
	operator<<(T const & t) {
		m_stream << t;
		return *this;
	}
	Error &
	operator<<(std::ostream&(*f)(std::ostream&)) {
		m_stream << f;
		return *this;
	}
	Error &
	operator<<(std::ios_base&(*f)(std::ios_base&)) {
		m_stream << f;
		return *this;
	}
private:
//MessageSender ap;
	Error(Error const&); // Change log 9
	Error& operator=(Error const&);
	std::stringstream m_stream;

};

// after printing out the message, this logger will terminate the application
// very useful for definately Fatal conditions
class Fatal {
public:
	explicit Fatal() {
		m_stream << "FAIL: ";

	}

// called once the chain of << operators is done...
	~Fatal() {
		_internal_log(m_stream.str(), MessageLevel::Critical);
		// todo: throw FatalException(m_stream.str());
		assert(false);
	}

	template<class T>
	Fatal &
	operator<<(T const & t) {
		m_stream << t;
		return *this;
	}
	Fatal &
	operator<<(std::ostream&(*f)(std::ostream&)) {
		m_stream << f;
		return *this;
	}
	Fatal &
	operator<<(std::ios_base&(*f)(std::ios_base&)) {
		m_stream << f;
		return *this;
	}
protected:
//MessageSender ap;
	Fatal(Fatal const&); // Change log 9
	Fatal& operator=(Fatal const&);
	std::stringstream m_stream;

};

class NotImplemented: Fatal {
public:
	explicit NotImplemented() {
		m_stream << "Method not implemented";
	}
};

}
