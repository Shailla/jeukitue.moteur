/*
 * PluginButtonProxy.h
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#ifndef PLUGINBUTTONPROXY_H_
#define PLUGINBUTTONPROXY_H_

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "lunar.h"

#include "plugin/lua/proxy/LunarProxy.h"
#include "plugin/api/gui/listener/ButtonPressedListener.h"

namespace JktPlugin {

class PluginContext;
class PluginButton;

class PluginButtonProxy : public LunarProxy, public ButtonPressedListener {
public:
	static const char className[];
	static Lunar<PluginButtonProxy>::RegType methods[];

private:
	PluginContext* _pluginContext;
	PluginButton* _pluginButton;

public:
	PluginButtonProxy(lua_State* L);
	PluginButtonProxy(PluginContext* pluginContext);
	virtual ~PluginButtonProxy();

	void buttonPressedEvent();
	int push(lua_State* L);
	void setWrappedObject(PluginButton* pluginButton);

	int expand(lua_State* L);
	int expandHoriz(lua_State* L);
	int expandVert(lua_State* L);
};

} /* namespace JktPlugin */
#endif /* PLUGINBUTTONPROXY_H_ */
