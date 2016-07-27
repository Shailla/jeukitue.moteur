/*
 * PlayerZoneDetectorProxy.h
 *
 *  Created on: 26 juil. 2016
 *      Author: Erwin
 */

#ifndef SRC_PLUGIN_LUA_PROXY_MAP_PLUGINPLAYERZONEDETECTORPROXY_H_
#define SRC_PLUGIN_LUA_PROXY_MAP_PLUGINPLAYERZONEDETECTORPROXY_H_

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "lunar.h"

#include "plugin/lua/proxy/LunarProxy.h"
#include "spatial/objet/CheckPlayerInZone.h"

namespace jkt {

class PluginPlayerZoneDetectorProxy : public LunarProxy {
	CheckPlayerInZone* _playerZoneDetector;

public:
	static const char className[];
	static Lunar<PluginPlayerZoneDetectorProxy>::RegType methods[];

	PluginPlayerZoneDetectorProxy(lua_State* L);
	PluginPlayerZoneDetectorProxy(CheckPlayerInZone* playerZoneDetector);
	virtual ~PluginPlayerZoneDetectorProxy();

	int push(lua_State* L);

	int setVisibility(lua_State *L);
};

} /* namespace jkt */

#endif /* SRC_PLUGIN_LUA_PROXY_MAP_PLUGINPLAYERZONEDETECTORPROXY_H_ */
