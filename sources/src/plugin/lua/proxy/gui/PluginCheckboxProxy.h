/*
 * PluginCheckboxProxy.h
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#ifndef PLUGINCHECKBOXPROXY_H_
#define PLUGINCHECKBOXPROXY_H_

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "lunar.h"

#include "plugin/lua/proxy/LunarProxy.h"
#include "plugin/api/gui/PluginCheckbox.h"

namespace jkt {

class PluginCheckboxProxy : public LunarProxy {
public:
	static const char className[];
	static Lunar<PluginCheckboxProxy>::RegType methods[];

private:
	PluginCheckbox* _pluginCheckbox;

public:
	PluginCheckboxProxy(lua_State* L);
	PluginCheckboxProxy(PluginCheckbox* pluginCheckbox);
	virtual ~PluginCheckboxProxy();

	int push(lua_State* L);
	int getValue(lua_State* L);
	int setValue(lua_State* L);
};

} /* namespace jkt */
#endif /* PLUGINCHECKBOXPROXY_H_ */
