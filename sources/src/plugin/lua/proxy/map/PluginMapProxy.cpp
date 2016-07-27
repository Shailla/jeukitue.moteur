/*
 * PluginMapProxy.cpp
 *
 *  Created on: 26 juil. 2016
 *      Author: Erwin
 */

#include "plugin/lua/LuaUtils.h"
#include "main/Fabrique.h"
#include "spatial/objet/CheckPlayerInZone.h"
#include "spatial/Map.h"
#include "plugin/lua/proxy/map/PluginPlayerZoneDetectorProxy.h"

#include "plugin/lua/proxy/map/PluginMapProxy.h"

namespace jkt {

const char PluginMapProxy::className[] = "Map";

Lunar<PluginMapProxy>::RegType PluginMapProxy::methods[] = {
		{"createPlayerZoneDetector", &PluginMapProxy::createPlayerZoneDetector},
		{0}
};

PluginMapProxy::PluginMapProxy(lua_State* L) {
	_map = 0;
}

PluginMapProxy::PluginMapProxy(CMap* map) {
	_map = map;
}

PluginMapProxy::~PluginMapProxy() {
}

int PluginMapProxy::push(lua_State* L) {
	return Lunar<PluginMapProxy>::push(L, this);
}

/**
 * Crée une zone virtuelle, quand le joueur passe dedans un événement est déclenché et les plugins sont notifiés
 */
int PluginMapProxy::createPlayerZoneDetector(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 7, LUA_PARAM_STRING, LUA_PARAM_NUMBER, LUA_PARAM_NUMBER, LUA_PARAM_NUMBER, LUA_PARAM_NUMBER, LUA_PARAM_NUMBER, LUA_PARAM_NUMBER)) {
		string detectorId = lua_tostring(L, 1);
		float xMin = lua_tonumber(L, 1);
		float xMax = lua_tonumber(L, 1);
		float yMin = lua_tonumber(L, 1);
		float yMax = lua_tonumber(L, 1);
		float zMin = lua_tonumber(L, 1);
		float zMax = lua_tonumber(L, 1);

		if(_map) {
			CheckPlayerInZone* detector = new CheckPlayerInZone(_map, detectorId, xMin, xMax, yMin, yMax, zMin, zMax);
			_map->add(detector);

			PluginPlayerZoneDetectorProxy* detectorProxy = new PluginPlayerZoneDetectorProxy(detector);
			return Lunar<PluginPlayerZoneDetectorProxy>::push(L, detectorProxy);
		}
		else {
			Fabrique::getPluginEngine()->getPluginContext(L)->logScriptError("Pas de Map active");
			return 0;
		}
	}

	return 0;
}

} /* namespace jkt */
