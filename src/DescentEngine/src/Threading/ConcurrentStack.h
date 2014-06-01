#pragma once

#pragma once

#include <boost/noncopyable.hpp>

#include <atomic>
#include <functional>

/*
 class ConcurrentLambda {
 public:
 typedef std::function<void()> this_lambda;
 std::atomic<bool> m_lmdSet;

 ConcurrentLambda() {
 m_lmdSet = false;
 }

 // important: new sets must override old ones
 void set(this_lambda const& ld) {
 m_lmd = ld;
 m_lmdSet = true;
 }

 bool execute() {

 // do thread safe compare & set here ...
 if

 return true;
 }

 this_lambda m_lmd;
 };
 */
template<class TItem>
class ConcurrentStack {
public:
	ConcurrentStack() {

	}

	/*TItem & get() {

	 }*/

	void put(TItem & itm) {

	}
};

template<class TItem>
class ConcurrentUsage: boost::noncopyable {
public:

	ConcurrentUsage(ConcurrentStack<TItem> & st) :
			m_st(st) {
	}

	~ConcurrentUsage() {

	}

private:
	ConcurrentStack<TItem> & m_st;
};
