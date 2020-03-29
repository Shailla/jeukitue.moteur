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
#include "plugin/PluginEngine.h"

#include "plugin/lua/proxy/map/PluginMapProxy.h"

using namespace std;

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
	Lunar<PluginMapProxy>::push(L, this);
	return 1;
}

/**
 * Crée une zone virtuelle, quand le joueur passe dedans un événement est déclenché et les plugins sont notifiés
 */
int PluginMapProxy::createPlayerZoneDetector(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 7, LUA_PARAM_STRING, LUA_PARAM_NUMBER, LUA_PARAM_NUMBER, LUA_PARAM_NUMBER, LUA_PARAM_NUMBER, LUA_PARAM_NUMBER, LUA_PARAM_NUMBER)) {
		string detectorId = lua_tostring(L, 1);
		float x1 = lua_tonumber(L, 2);
		float x2 = lua_tonumber(L, 3);
		float y1 = lua_tonumber(L, 4);
		float y2 = lua_tonumber(L, 5);
		float z1 = lua_tonumber(L, 6);
		float z2 = lua_tonumber(L, 7);

		float xMin = (x1>x2)?x2:x1;
		float xMax = (x1>x2)?x1:x2;
		float yMin = (y1>y2)?y2:y1;
		float yMax = (y1>y2)?y1:y2;
		float zMin = (z1>z2)?z2:z1;
		float zMax = (z1>z2)?z1:z2;

		if(_map) {
			CheckPlayerInZone* detector = new CheckPlayerInZone(_map, detectorId, xMin, xMax, yMin, yMax, zMin, zMax);
			_map->add(detector);

			PluginPlayerZoneDetectorProxy* detectorProxy = new PluginPlayerZoneDetectorProxy(detector);
			Lunar<PluginPlayerZoneDetectorProxy>::push(L, detectorProxy);
			return 1;
		}
		else {
			Fabrique::getPluginEngine()->getPluginContext(L)->logScriptError("Pas de Map active");
			return 0;
		}
	}

	return 0;
}

} /* namespace jkt */
