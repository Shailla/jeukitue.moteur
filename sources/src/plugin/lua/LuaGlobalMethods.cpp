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

namespace jkt {

const char* LuaGlobalMethods::REFRESH_EVENT_TYPE = "refresh";

LuaGlobalMethods::LuaGlobalMethods() {
}

LuaGlobalMethods::~LuaGlobalMethods() {
}

int LuaGlobalMethods::log(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 1, LUA_PARAM_STRING)) {
		PluginContext* pluginContext = Fabrique::getPluginEngine()->getGlobalPluginContext(L);

		if(pluginContext) {
			pluginContext->logUser(lua_tostring(L, 1));
		}
		else {
			pluginContext = Fabrique::getPluginEngine()->getMapPluginContext(L);

			if(pluginContext) {
				pluginContext->logUser(lua_tostring(L, 1));
			}
		}
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
			PluginContext* pluginContext = Fabrique::getPluginEngine()->getGlobalPluginContext(L);
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

/**
 * Subscribe the plugin to receive events of a specific type
 */
int LuaGlobalMethods::subscribeEvents(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 1, LUA_PARAM_STRING)) {
		PluginContext* pluginContext = Fabrique::getPluginEngine()->getPluginContext(L);
		string type = lua_tostring(L, 1);

		if(type == REFRESH_EVENT_TYPE) {
			pluginContext->subscribeRefreshEvents(true);
		}
		else {
			pluginContext->logError("Unknown event subscription type : '" + type + "'");
		}
	}

	return 0;
}

/**
 * Cr�e une zone virtuelle, quand le joueur passe dedans un �v�nement est d�clench� et les plugins sont notifi�s
 */
int LuaGlobalMethods::createPlayerZoneDetector(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 7, LUA_PARAM_STRING, LUA_PARAM_NUMBER, LUA_PARAM_NUMBER, LUA_PARAM_NUMBER, LUA_PARAM_NUMBER, LUA_PARAM_NUMBER, LUA_PARAM_NUMBER)) {
		string detectorId = lua_tostring(L, 1);
		double xMin = lua_tonumber(L, -1);
		double xMax = lua_tonumber(L, -1);
		double yMin = lua_tonumber(L, -1);
		double yMax = lua_tonumber(L, -1);
		double zMin = lua_tonumber(L, -1);
		double zMax = lua_tonumber(L, -1);


	}

	return 0;
}

} /* namespace jkt */
