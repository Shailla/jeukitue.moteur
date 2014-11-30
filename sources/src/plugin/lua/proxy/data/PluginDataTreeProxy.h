/*
 * PluginDataTreeProxy.h
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#ifndef PLUGINDATATREEPROXY_H_
#define PLUGINDATATREEPROXY_H_

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

namespace JktPlugin {

class PluginDataTreeProxy {
	static PluginDataTreeProxy* _this;
public:
	static const char className[];
	static Lunar<PluginDataTreeProxy>::RegType methods[];

	static int getMapDataTree(lua_State *L);
	static PluginDataTreeProxy* getInstance();

	PluginDataTreeProxy();
	PluginDataTreeProxy(lua_State* L);

	int createValeur(lua_State *L);
	int createBranche(lua_State *L);
};

} /* namespace JktPlugin */
#endif /* PLUGINDATATREEPROXY_H_ */
