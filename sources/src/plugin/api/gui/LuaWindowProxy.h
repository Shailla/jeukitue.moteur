/*
 * LuaWindowProxy.h
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#ifndef LUAWINDOWPROXY_H_
#define LUAWINDOWPROXY_H_

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "lunar.h"

#include "plugin/api/gui/LuaWindow.h"

namespace JktPlugin {

class LuaWindowProxy : private LuaWindow {
	friend class Lunar<LuaWindowProxy>;
	static const char className[];
	static Lunar<LuaWindowProxy>::RegType methods[];
public:
	LuaWindowProxy(lua_State* L);
	virtual ~LuaWindowProxy();

	int setSize(lua_State* L);
	int setTitle(lua_State* L);
	int show(lua_State* L);
	int hide(lua_State* L);
};

} /* namespace JktPlugin */
#endif /* LUAWINDOWPROXY_H_ */
