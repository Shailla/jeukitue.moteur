/*
 * PluginTabProxy.h
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#ifndef PLUGINBOXPROXY_H_
#define PLUGINBOXPROXY_H_

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "lunar.h"

#include "plugin/lua/proxy/LunarProxy.h"
#include "plugin/api/gui/PluginBox.h"
#include "plugin/lua/proxy/gui/AbstractPluginPanelProxy.h"

namespace JktPlugin {

class PluginBoxProxy : public LunarProxy, public AbstractPluginPanelProxy {
public:
	static const char className[];
	static Lunar<PluginBoxProxy>::RegType methods[];

private:
	PluginBox* _pluginBox;

public:
	PluginBoxProxy(lua_State* L);
	PluginBoxProxy(PluginContext* pluginContext, PluginBox* pluginBox);
	virtual ~PluginBoxProxy();

	int push(lua_State* L);
};

} /* namespace JktPlugin */
#endif /* PLUGINBOXPROXY_H_ */
