/*
 * LuaGlobalMethods.cpp
 *
 *  Created on: 20 mai 2012
 *      Author: Erwin
 */

#include "menu/Controller.h"
#include "plugin/PluginEngine.h"
#include "plugin/lua/LuaUtils.h"
#include "main/Fabrique.h"
#include "plugin/lua/LuaGlobalMethods.h"

namespace JktPlugin {

LuaGlobalMethods::LuaGlobalMethods() {
}

LuaGlobalMethods::~LuaGlobalMethods() {
}

int LuaGlobalMethods::log(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 1, LUA_PARAM_STRING)) {
		PluginContext* pluginContext = Fabrique::getPluginEngine()->getPluginContext(L);
		pluginContext->logUser(lua_tostring(L, 1));
	}

	return 0;
}

/**
 * Send a new event from the plugin to the JKT event controller
 */
int LuaGlobalMethods::pushEvent(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 1, LUA_PARAM_NUMBER)) {
		int pluginActionId = lua_tonumber(L, 1);

		if(pluginActionId < Controller::Action::PLUGIN_BASE_ID) {
			PluginContext* pluginContext = Fabrique::getPluginEngine()->getPluginContext(L);
			string message = "Illegal use of an internal action id (";
			message += pluginActionId;
			message += ")";
			pluginContext->logError(message);
		}
		else {
			AG_Event event;
			AG_EventArgs(&event, "%i", pluginActionId);
			Controller::executeAction(&event);
		}
	}

	return 0;
}

} /* namespace JktPlugin */
