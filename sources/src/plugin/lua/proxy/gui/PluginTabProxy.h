/*
 * PluginTabProxy.h
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#ifndef PLUGINTABPROXY_H_
#define PLUGINTABPROXY_H_

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "lunar.h"

#include "plugin/lua/proxy/LunarProxy.h"
#include "plugin/lua/proxy/gui/AbstractPluginPanelProxy.h"

namespace JktPlugin {

class PluginContext;
class PluginTab;

class PluginTabProxy : public LunarProxy, public AbstractPluginPanelProxy {
public:
	static const char className[];
	static Lunar<PluginTabProxy>::RegType methods[];

private:
	PluginContext* _pluginContext;
	PluginTab* _pluginTab;

public:
	PluginTabProxy(lua_State* L);
	PluginTabProxy(PluginContext* pluginContext, PluginTab* pluginTab);
	virtual ~PluginTabProxy();

	int push(lua_State* L);
};

} /* namespace JktPlugin */
#endif /* PLUGINTABPROXY_H_ */
