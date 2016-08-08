/*
 * PluginDataTreeProxy.h
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#ifndef PLUGINGAMEPROXY_H_
#define PLUGINGAMEPROXY_H_

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "main/Game.h"

namespace jkt {

class PluginGameProxy : public LunarProxy {
	CGame* _game;

public:
	static const char className[];
	static Lunar<PluginGameProxy>::RegType methods[];

	static int getGame(lua_State *L);

	PluginGameProxy(CGame* game);
	PluginGameProxy(lua_State* L);

	int getMap(lua_State *L);
	int getDataTree(lua_State *L);
	int getPlayer(lua_State *L);

	int push(lua_State* L);
};

} /* namespace jkt */
#endif /* PLUGINGAMEPROXY_H_ */
