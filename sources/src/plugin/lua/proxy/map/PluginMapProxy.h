/*
 * PluginMapProxy.h
 *
 *  Created on: 26 juil. 2016
 *      Author: Erwin
 */

#ifndef SRC_PLUGIN_MAP_PROXY_H_
#define SRC_PLUGIN_MAP_PROXY_H_

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "lunar.h"

#include "plugin/lua/proxy/LunarProxy.h"

#include "spatial/Map.h"

namespace jkt {

class PluginMapProxy : public LunarProxy {
	CMap* _map;

public:
	static const char className[];
	static Lunar<PluginMapProxy>::RegType methods[];

	PluginMapProxy(lua_State* L);
	PluginMapProxy(CMap* map);
	virtual ~PluginMapProxy();

	int push(lua_State* L);

	// Crée une zone virtuelle, quand le joueur passe dedans un événement est déclenché et les plugins sont notifiés
	int createPlayerZoneDetector(lua_State* L);
};

} /* namespace jkt */

#endif /* SRC_PLUGIN_MAP_PROXY_H_ */
