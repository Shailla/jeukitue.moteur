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

class DataTree;

namespace jkt {

class PluginDataTreeProxy {
	DataTree* _dataTree;
public:
	static const char className[];
	static Lunar<PluginDataTreeProxy>::RegType methods[];

	static int getDataTree(lua_State *L);

	PluginDataTreeProxy(DataTree* dataTree);
	PluginDataTreeProxy(lua_State* L);

	// Branches operations
	int getBranche(lua_State *L);
	int createBranche(lua_State *L);

	// Valeurs operations
	int createValeur(lua_State *L);
};

} /* namespace jkt */
#endif /* PLUGINDATATREEPROXY_H_ */
