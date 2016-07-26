/*
 * PlayerZoneDetectorProxy.cpp
 *
 *  Created on: 26 juil. 2016
 *      Author: Erwin
 */

#include "plugin/lua/LuaUtils.h"

#include "plugin/lua/proxy/PluginPlayerZoneDetectorProxy.h"

namespace jkt {

const char PluginPlayerZoneDetectorProxy::className[] = "PlayerZoneDetector";

Lunar<PluginPlayerZoneDetectorProxy>::RegType PluginPlayerZoneDetectorProxy::methods[] = {
		{"setVisibility", &PluginPlayerZoneDetectorProxy::setVisibility},
		{0}
};

PluginPlayerZoneDetectorProxy::PluginPlayerZoneDetectorProxy(lua_State* L) {
	_playerZoneDetector = 0;
}

PluginPlayerZoneDetectorProxy::PluginPlayerZoneDetectorProxy(CheckPlayerInZone* playerZoneDetector) {
	_playerZoneDetector = playerZoneDetector;
}

PluginPlayerZoneDetectorProxy::~PluginPlayerZoneDetectorProxy() {
}

int PluginPlayerZoneDetectorProxy::push(lua_State* L) {
	return Lunar<PluginPlayerZoneDetectorProxy>::push(L, this);
}

int PluginPlayerZoneDetectorProxy::setVisibility(lua_State *L) {
	LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 1, LUA_PARAM_BOOLEAN);

	// Get input values
	int index = 1;

	bool visibility = lua_toboolean(L, index++);

	if(_playerZoneDetector) {
		_playerZoneDetector->setVisibility(visibility);
	}

	return 1;
}

} /* namespace jkt */
