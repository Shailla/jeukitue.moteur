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

#include "lunaFive.h"

#include "plugin/lua/proxy/gui/AbstractPluginPanelProxy.h"

namespace JktPlugin {

class PluginContext;
class PluginTab;

class PluginTabProxy : public AbstractPluginPanelProxy {
	friend class Luna<PluginTabProxy>;
	static const char className[];
	static Luna<PluginTabProxy>::FunctionType methods[];

	PluginContext* _pluginContext;
	PluginTab* _pluginTab;
public:
	PluginTabProxy(lua_State* L);
	PluginTabProxy(PluginContext* pluginContext, PluginTab* pluginTab);
	virtual ~PluginTabProxy();
};

} /* namespace JktPlugin */
#endif /* PLUGINTABPROXY_H_ */
