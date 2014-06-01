#pragma once

#include <functional>

#include "../Log.h"

#ifdef USE_THREADING_CPP11

// c++11 threading
#include <thread>
typedef std::thread PortableThread;

#endif

#ifdef USE_THREADING_NULL

class PortableThread {
public:

	template<typename _Callable, typename ... _Args>
	explicit PortableThread(_Callable&& __f, _Args&&... __args)
	{
		/*   _M_start_thread(_M_make_routine(std::bind<void>(
		 std::forward<_Callable>(__f),
		 std::forward<_Args>(__args)...)));*/
		logging::Fatal() << "not impemented for Android yet";
	}
private:
};

#endif

