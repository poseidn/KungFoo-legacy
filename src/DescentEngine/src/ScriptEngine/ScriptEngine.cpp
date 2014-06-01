#include "ScriptEngine.h"

#ifdef DESCENT_FILE_WATCH

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>

#endif

#include "../ResourceEngine/ResourceEngine.h"

#ifdef DESCENT_FILE_WATCH
void Script::checkWatch() {
	m_scriptEngine->checkWatch(*this);
}
#else
void Script::checkWatch() {
}
#endif

ScriptEngine::ScriptEngine(ResourceEngine & re/*, RegisterList const& regList*/) :
		m_re(re) {

#ifdef DESCENT_FILE_WATCH
	//m_inotifySession = inotify_init1 ( O_NONBLOCK );
	m_inotify.SetNonBlock(true);
#endif

	m_defaultState = luaL_newstate();

	luabind::open(m_defaultState);

	luabind::module(m_defaultState)[
	//
	luabind::class_<LuaLogging>("Logging")
	//
	.def(luabind::constructor<>())
	//
	.def("Info", &LuaLogging::Info)
	//
	.def("Fatal", &LuaLogging::Fatal)
	//
	];

	/*	// register custom objects
	 for (auto const& reg : regList) {
	 reg(m_defaultState);
	 }*/

	auto retCode = luaL_dostring(m_defaultState, "logging = Logging()");
	assert(retCode == 0);

	// print does not work when in embedded mode
	retCode = luaL_dostring(m_defaultState, "function bootstrapEnv()\n"
			"  logging:Info('Lua script started')\n"
			"  return 0\n"
			"end\n"
			"bootstrapEnv()\n");
	assert(retCode == 0);

	retCode = luabind::call_function<int>(m_defaultState, "bootstrapEnv");
	assert(retCode == 0);

}

ScriptEngine::~ScriptEngine() {
	lua_close(m_defaultState);

#ifdef DESCENT_FILE_WATCH
	for (auto w : m_watches) {
		m_inotify.Remove(w.get());
	}
#endif
}

void ScriptEngine::addObjectRegister(ObjectRegisterLambda const & reg) {
	m_registerList.push_back(reg);
}

Script ScriptEngine::loadScript(std::string const& scriptName) {

	logging::Info() << "Loading Script " << scriptName;
	std::string filePath = m_re.getScriptPath(scriptName);

	Script sc;
	sc.m_scriptEngine = this;
	sc.m_fileName = filePath;
	sc.m_scriptName = scriptName;

	registerWatch(filePath, sc);

	bootstrapState(sc);
	injectScript(sc);

	return sc;
}

void ScriptEngine::injectScript(Script & sc) {
	std::string code = m_re.loadScript(sc.m_scriptName);
	bootstrapScript(sc, code);

}

void ScriptEngine::bootstrapScript(Script & sc, std::string const& code) {

	luabind::open(sc.m_luaState);

	auto retCode = luaL_dostring(sc.m_luaState, code.c_str());

	if (retCode) {
		logging::Error() << "Lua script " << sc.m_scriptName << " failed to load: " << std::endl
		<< lua_tostring(sc.m_luaState,-1);
		lua_pop(sc.m_luaState, 1); /* pop error message from the stack */
		sc.m_inactive = true;
	} else {
		sc.m_inactive = false;
	}
}

void ScriptEngine::checkWatch(Script & sc) {
#ifdef DESCENT_FILE_WATCH

	// does not actually wait, because we are in
	// non-blocking mode
	m_inotify.WaitForEvents();

	size_t count = m_inotify.GetEventCount();
	while (count > 0) {
		InotifyEvent event;
		bool got_event = m_inotify.GetEvent(&event);

		if (got_event) {
			if (event.GetMask() & IN_MODIFY) {
				logging::Info() << "File " << sc.m_fileName << " changed on disk, reloading";
			}

			injectScript(sc);
		}
		count--;
	}
#endif
}

void ScriptEngine::bootstrapState(Script & sc) {

	sc.m_luaState = luaL_newstate();
	assert(sc.m_luaState != nullptr);

	luabind::open(sc.m_luaState);
	luabind::module(sc.m_luaState)[
	//
	luabind::class_<LuaLogging>("Logging")
	//
	.def(luabind::constructor<>())
	//
	.def("Info", &LuaLogging::Info)
	//
	.def("Fatal", &LuaLogging::Fatal)];

	// register custom objects
	for (auto const& reg : m_registerList) {
		reg(sc.m_luaState);
	}

	auto retCode = luaL_dostring(sc.m_luaState, "logging = Logging()");
	assert(retCode == 0);
}

void ScriptEngine::registerWatch(std::string const& filePath, Script & sc) {
#ifdef DESCENT_FILE_WATCH
	logging::Info() << "Registering watcher for " << filePath;

	InotifyWatchPtr watch = std::make_shared< InotifyWatch >(filePath, IN_MODIFY);
	m_inotify.Add(watch.get());
	sc.m_inotifyWatch = watch;
	m_watches.push_back(watch);

#endif
}

