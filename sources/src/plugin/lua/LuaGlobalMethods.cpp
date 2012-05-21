/*
 * LuaGlobalMethods.cpp
 *
 *  Created on: 20 mai 2012
 *      Author: Erwin
 */

#include "plugin/PluginEngine.h"
#include "plugin/lua/LuaUtils.h"

#include "plugin/lua/LuaGlobalMethods.h"

namespace JktPlugin {

LuaGlobalMethods::LuaGlobalMethods() {
}

LuaGlobalMethods::~LuaGlobalMethods() {
}

int LuaGlobalMethods::log(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 1, LUA_PARAM_STRING)) {
		PluginContext* pluginContext = PluginEngine::getPluginContext(L);
		pluginContext->logUser(lua_tostring(L, 1));
	}

	return 0;
}

} /* namespace JktPlugin */
