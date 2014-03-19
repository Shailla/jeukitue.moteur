/*
 * PluginNotebookProxy.h
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#ifndef PLUGINNOTEBOOKPROXY_H_
#define PLUGINNOTEBOOKPROXY_H_

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "lunar.h"

#include "plugin/lua/proxy/LunarProxy.h"

namespace JktPlugin {

class PluginContext;
class PluginNotebook;

class PluginNotebookProxy :public LunarProxy{
public:
	static const char className[];
	static Lunar<PluginNotebookProxy>::RegType methods[];

private:
	PluginContext* _pluginContext;
	PluginNotebook* _pluginNotebook;

public:
	PluginNotebookProxy(lua_State* L);
	PluginNotebookProxy(PluginContext* pluginContext, PluginNotebook* pluginNotebook);
	virtual ~PluginNotebookProxy();

	int push(lua_State* L);

	// Enfants
	int addTab(lua_State* L);
};

} /* namespace JktPlugin */
#endif /* PLUGINNOTEBOOKPROXY_H_ */
