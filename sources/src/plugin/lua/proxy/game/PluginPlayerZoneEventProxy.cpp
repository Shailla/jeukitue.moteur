/*
 * PluginPlayerZoneEventProxy.cpp
 *
 *  Created on: 7 août 2016
 *      Author: VGDJ7997
 */

#include "plugin/lua/proxy/game/PluginPlayerZoneEventProxy.h"

namespace jkt {

const char PluginPlayerZoneEventProxy::className[] = "PlayerZoneEventInfo";

Lunar<PluginPlayerZoneEventProxy>::RegType PluginPlayerZoneEventProxy::methods[] = {
		{"getPlayerId", &PluginPlayerZoneEventProxy::getPlayerId},
		{0}
};

PluginPlayerZoneEventProxy::PluginPlayerZoneEventProxy(int playerId) {
	_playerId = playerId;
}

PluginPlayerZoneEventProxy::PluginPlayerZoneEventProxy(lua_State* L) {
	_playerId = -1;
}

PluginPlayerZoneEventProxy::~PluginPlayerZoneEventProxy() {
}

int PluginPlayerZoneEventProxy::getPlayerId(lua_State* L) {
	lua_pushnumber(L, _playerId);

	return 1;
}

int PluginPlayerZoneEventProxy::push(lua_State* L) {
	return Lunar<PluginPlayerZoneEventProxy>::push(L, this);
}

} /* namespace jkt */
