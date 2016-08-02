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
		{0}
};

PluginPlayerProxy::PluginPlayerProxy() {
}

PluginPlayerProxy::PluginPlayerProxy(lua_State* L) {

}

PluginPlayerProxy::~PluginPlayerProxy() {
}

int PluginPlayerProxy::push(lua_State* L) {
	return Lunar<PluginPlayerProxy>::push(L, this);
}

} /* namespace jkt */
