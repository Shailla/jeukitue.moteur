/*
 * PluginPlayerProxy.h
 *
 *  Created on: 31 juil. 2016
 *      Author: VGDJ7997
 */

#ifndef SRC_PLUGIN_LUA_PROXY_GAME_PLUGINPLAYERPROXY_H_
#define SRC_PLUGIN_LUA_PROXY_GAME_PLUGINPLAYERPROXY_H_

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "lunar.h"

#include "main/Player.h"

#include "plugin/lua/proxy/LunarProxy.h"

namespace jkt {

class PluginPlayerProxy : public LunarProxy {
	CPlayer* _player;
public:
	static const char className[];
	static Lunar<PluginPlayerProxy>::RegType methods[];

public:
	PluginPlayerProxy(CPlayer* player);
	PluginPlayerProxy(lua_State* L);
	virtual ~PluginPlayerProxy();

	int getName(lua_State* L);

	int push(lua_State* L);
};

} /* namespace JktNet */

#endif /* SRC_PLUGIN_LUA_PROXY_GAME_PLUGINPLAYERPROXY_H_ */
