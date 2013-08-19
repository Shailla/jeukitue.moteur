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

#include "lunaFive.h"

#include "plugin/lua/proxy/gui/AbstractPluginPanelProxy.h"

#include "plugin/api/gui/PluginWindow.h"

namespace JktPlugin {

class PluginContext;

class PluginWindowProxy : public AbstractPluginPanelProxy {
	friend class Luna<PluginWindowProxy>;
	static const char *className;
	static const Luna<PluginWindowProxy>::FunctionType methods[];
	static const Luna<PluginWindowProxy>::PropertyType properties[];

	bool isExisting; // This is used by Luna to see whether it's been created by createFromExisting.  Don't set it.
	bool isPrecious; // This is used to tell Luna not to garbage collect the object, in case other objects might reference it.  Set it in your classes constructor.

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
