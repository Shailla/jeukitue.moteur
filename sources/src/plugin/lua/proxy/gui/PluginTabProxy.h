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

#include "plugin/lua/proxy/gui/AbstractPluginPanelProxy.h"

namespace JktPlugin {

class PluginContext;
class PluginTab;

class PluginTabProxy : public AbstractPluginPanelProxy {
	friend class Lunar<PluginTabProxy>;
	static const char className[];
	static Lunar<PluginTabProxy>::RegType methods[];

	PluginContext* _pluginContext;
	PluginTab* _pluginTab;
public:
	PluginTabProxy(lua_State* L);
	PluginTabProxy(PluginContext* pluginContext, PluginTab* pluginTab);
	virtual ~PluginTabProxy();
};

} /* namespace JktPlugin */
#endif /* PLUGINTABPROXY_H_ */
