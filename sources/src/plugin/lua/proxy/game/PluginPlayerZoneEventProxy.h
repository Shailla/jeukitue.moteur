/*
 * PluginPlayerZoneEventProxy.h
 *
 *  Created on: 7 août 2016
 *      Author: VGDJ7997
 */

#ifndef SRC_PLUGIN_LUA_PROXY_GAME_PLUGINPLAYERZONEEVENTPROXY_H_
#define SRC_PLUGIN_LUA_PROXY_GAME_PLUGINPLAYERZONEEVENTPROXY_H_

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "lunar.h"

#include "plugin/lua/proxy/LunarProxy.h"


namespace jkt {

class PluginPlayerZoneEventProxy : public LunarProxy {
	int _playerId;
public:
	static const char className[];
	static Lunar<PluginPlayerZoneEventProxy>::RegType methods[];

	PluginPlayerZoneEventProxy(int playerId);
	virtual ~PluginPlayerZoneEventProxy();

	int getPlayerId(lua_State* L);
	int push(lua_State* L);
};

} /* namespace jkt */

#endif /* SRC_PLUGIN_LUA_PROXY_GAME_PLUGINPLAYERZONEEVENTPROXY_H_ */
