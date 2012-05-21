/*
 * PluginWindowProxy.h
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#ifndef PLUGINWINDOWPROXY_H_
#define PLUGINWINDOWPROXY_H_

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "lunar.h"

#include "plugin/lua/proxy/gui/AbstractPluginPanelProxy.h"

#include "plugin/api/gui/PluginWindow.h"

namespace JktPlugin {

class PluginContext;

class PluginWindowProxy : public AbstractPluginPanelProxy {
	friend class Lunar<PluginWindowProxy>;
	static const char className[];
	static Lunar<PluginWindowProxy>::RegType methods[];

	PluginContext* _pluginContext;
	PluginWindow* _pluginWindow;
public:
	PluginWindowProxy(lua_State* L);
	virtual ~PluginWindowProxy();

	// Visibilité
	int show(lua_State* L);
	int hide(lua_State* L);

	// Apparence
	int setTitle(lua_State* L);
	int setSize(lua_State* L);
	int setPosition(lua_State* L);
};

} /* namespace JktPlugin */
#endif /* PLUGINWINDOWPROXY_H_ */
