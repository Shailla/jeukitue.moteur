/*
 * PluginPlayerProxy.cpp
 *
 *  Created on: 31 juil. 2016
 *      Author: VGDJ7997
 */

#include "plugin/lua/proxy/game/PluginPlayerProxy.h"

namespace jkt {

const char PluginPlayerProxy::className[] = "Player";

Lunar<PluginPlayerProxy>::RegType PluginPlayerProxy::methods[] = {
		{"getName", &PluginPlayerProxy::getName},
		{0}
};

PluginPlayerProxy::PluginPlayerProxy(CPlayer* player) {
	_player = player;
}

PluginPlayerProxy::PluginPlayerProxy(lua_State* L) {
	_player = 0;
}

PluginPlayerProxy::~PluginPlayerProxy() {
}

int PluginPlayerProxy::getName(lua_State* L) {
	if(_player) {
		lua_pushstring(L, _player->getName().c_str());
		return 1;
	}
	else {
		return 0;
	}
}

int PluginPlayerProxy::push(lua_State* L) {
	return Lunar<PluginPlayerProxy>::push(L, this);
}

} /* namespace jkt */
