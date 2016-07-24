/*
 * PluginContext.h
 *
 *  Created on: 9 mai 2012
 *      Author: Erwin
 */

#ifndef PLUGINCONTEXT_H_
#define PLUGINCONTEXT_H_

#include <fstream>
#include <string>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "SDL.h"

#include "lunar.h"

#include "plugin/PluginWidgetEvent.h"
#include "plugin/PluginActionEvent.h"

namespace jkt {

class PluginContext {
	static const char* LOG_USER_PREFIX;
	static const char* LOG_INFO_PREFIX;
	static const char* LOG_ERROR_PREFIX;
	static const char* LOG_ERROR_LUA_PREFIX;

	std::ofstream _logFile;
	lua_State* _luaState;
	const std::string _pluginName;

	SDL_mutex* _dispatchEventMutex;

	bool _subscribedRefreshEvents;

public:
	PluginContext(lua_State* luaState, const std::string& pluginName, const std::string& pluginsDirectory);
	virtual ~PluginContext();

	lua_State* getLuaState();

	// Gestion des traces du plugin
	void logUser(const std::string& trace);
	void logInfo(const std::string& trace);
	void logError(const std::string& trace);
	void logScriptError(const std::string& trace);
	void logLuaError(const int status);

	void dispatchEvent(PluginWidgetEvent* event);
	void dispatchEvent(const PluginActionEvent& event);
	void subscribeRefreshEvents(const bool state);
	bool isSubscribedRefreshEvents() const;
};

} /* namespace jkt */
#endif /* PLUGINCONTEXT_H_ */
