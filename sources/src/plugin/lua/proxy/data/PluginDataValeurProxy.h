/*
 * PluginDataValeurProxy.h
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#ifndef PLUGINDATAVALEURPROXY_H_
#define PLUGINDATAVALEURPROXY_H_

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

class Valeur;

namespace jkt {

class PluginDataValeurProxy {
	Valeur* _valeur;

public:
	static const char className[];
	static Lunar<PluginDataValeurProxy>::RegType methods[];

	PluginDataValeurProxy(Valeur* valeur);
	PluginDataValeurProxy(lua_State* L);

	int getValue(lua_State *L);
	int setValue(lua_State *L);
	int getBrancheId(lua_State *L);
	int getValeurFullId(lua_State *L);
};

} /* namespace jkt */
#endif /* PLUGINDATAVALEURPROXY_H_ */
