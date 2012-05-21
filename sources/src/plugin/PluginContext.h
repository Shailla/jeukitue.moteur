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

#include "lunar.h"

namespace JktPlugin {

class PluginContext {
	static const char* LOG_USER_PREFIX;
	static const char* LOG_INFO_PREFIX;
	static const char* LOG_ERROR_PREFIX;
	static const char* LOG_ERROR_LUA_PREFIX;

	std::ofstream _logFile;
	lua_State* _luaState;
	const std::string _pluginName;
public:
	PluginContext(const std::string& pluginName, const std::string& pluginDirectory);
	virtual ~PluginContext();

	lua_State* getLuaState();
	void setLuaState(lua_State* luaState);

	// Gestion des traces du plugin
	void logUser(const std::string& trace);
	void logInfo(const std::string& trace);
	void logError(const std::string& trace);
	void logLuaError(const int status);
};

} /* namespace JktPlugin */
#endif /* PLUGINCONTEXT_H_ */
