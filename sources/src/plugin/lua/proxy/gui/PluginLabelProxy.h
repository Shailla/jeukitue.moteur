/*
 * PluginLabelProxy.h
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#ifndef PLUGINLABELPROXY_H_
#define PLUGINLABELPROXY_H_

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "lunar.h"

#include "plugin/lua/proxy/LunarProxy.h"

namespace JktPlugin {

class PluginContext;
class PluginLabel;

class PluginLabelProxy : public LunarProxy {
public:
	static const char className[];
	static Lunar<PluginLabelProxy>::RegType methods[];

private:
	PluginContext* _pluginContext;
	PluginLabel* _pluginLabel;

public:
	PluginLabelProxy(lua_State* L);
	PluginLabelProxy(PluginContext* pluginContext);
	virtual ~PluginLabelProxy();

	int push(lua_State* L);
	void setWrappedObject(PluginLabel* pluginLabel);

	int expand(lua_State* L);
	int expandHoriz(lua_State* L);
	int expandVert(lua_State* L);
};

} /* namespace JktPlugin */
#endif /* PLUGINLABELPROXY_H_ */
