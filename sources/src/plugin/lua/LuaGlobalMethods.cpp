/*
 * LuaGlobalMethods.cpp
 *
 *  Created on: 20 mai 2012
 *      Author: Erwin
 */

#include "main/Fabrique.h"
#include "spatial/objet/CheckPlayerInZone.h"
#include "spatial/Map.h"
#include "main/Game.h"
#include "menu/Controller.h"
#include "plugin/lua/proxy/PluginPlayerZoneDetectorProxy.h"
#include "plugin/PluginEngine.h"
#include "plugin/lua/LuaUtils.h"

#include "plugin/lua/LuaGlobalMethods.h"

extern CGame Game;

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
 * Crée une zone virtuelle, quand le joueur passe dedans un événement est déclenché et les plugins sont notifiés
 */
int LuaGlobalMethods::createPlayerZoneDetector(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 7, LUA_PARAM_STRING, LUA_PARAM_NUMBER, LUA_PARAM_NUMBER, LUA_PARAM_NUMBER, LUA_PARAM_NUMBER, LUA_PARAM_NUMBER, LUA_PARAM_NUMBER)) {
		string detectorId = lua_tostring(L, 1);
		float xMin = lua_tonumber(L, -1);
		float xMax = lua_tonumber(L, -1);
		float yMin = lua_tonumber(L, -1);
		float yMax = lua_tonumber(L, -1);
		float zMin = lua_tonumber(L, -1);
		float zMax = lua_tonumber(L, -1);

		CMap* map = Game.getMap();
		CheckPlayerInZone* detector = new CheckPlayerInZone(map, detectorId, xMin, xMax, yMin, yMax, zMin, zMax);
		map->add(detector);

		PluginPlayerZoneDetectorProxy* detectorProxy = new PluginPlayerZoneDetectorProxy(detector);
		return Lunar<PluginPlayerZoneDetectorProxy>::push(L, detectorProxy);
	}

	return 0;
}

} /* namespace jkt */
