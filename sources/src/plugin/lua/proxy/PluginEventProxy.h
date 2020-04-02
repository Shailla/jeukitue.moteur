/*
 * PluginEventProxy.h
 *
 *  Created on: 10 f�vr. 2014
 *      Author: vgdj7997
 */

#ifndef PLUGINEVENTPROXY_H_
#define PLUGINEVENTPROXY_H_

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <lunar.h>

#include "menu/Controller.h"
#include "plugin/PluginActionEvent.h"
#include "plugin/lua/proxy/LunarProxy.h"

namespace jkt {

class PluginEventProxy : public LunarProxy {
public:
	static const char className[];
	static Lunar<PluginEventProxy>::RegType methods[];

private:
	int _actionId;			// Controller::Action
	LunarProxy* _source;
	LunarProxy* _info;

public:
	PluginEventProxy(int actionId);
	PluginEventProxy(int actionId, LunarProxy* source, LunarProxy* info);
	PluginEventProxy(lua_State* L);
	~PluginEventProxy();

	int push(lua_State* L);
	int getSource(lua_State* L);
	void setInfo(LunarProxy* source);
	int getInfo(lua_State* L);
	int getActionId(lua_State* L);
};

} /* namespace jkt */

#endif /* PLUGINEVENTPROXY_H_ */
