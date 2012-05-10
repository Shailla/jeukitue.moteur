/*
 * PluginContext.h
 *
 *  Created on: 9 mai 2012
 *      Author: Erwin
 */

#ifndef PLUGINCONTEXT_H_
#define PLUGINCONTEXT_H_

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "lunar.h"

namespace JktPlugin {

class PluginContext {
	lua_State* _luaState;
public:
	PluginContext(lua_State* luaState);
	virtual ~PluginContext();

	lua_State* getLuaState();
};

} /* namespace JktLua */
#endif /* PLUGINCONTEXT_H_ */
