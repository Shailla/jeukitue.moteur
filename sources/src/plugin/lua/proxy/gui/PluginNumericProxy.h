/*
 * PluginNumericProxy.h
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#ifndef PLUGINNUMERICPROXY_H_
#define PLUGINNUMERICPROXY_H_

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "lunar.h"

#include "plugin/lua/proxy/LunarProxy.h"
#include "plugin/api/gui/PluginNumeric.h"

namespace jkt {

class PluginNumericProxy : public LunarProxy {
public:
	static const char className[];
	static Lunar<PluginNumericProxy>::RegType methods[];

private:
	PluginNumeric* _pluginNumeric;

public:
	PluginNumericProxy(lua_State* L);
	PluginNumericProxy(PluginNumeric* pluginNumeric);
	virtual ~PluginNumericProxy();

	int push(lua_State* L);
	int getValue(lua_State* L);
};

} /* namespace jkt */
#endif /* PLUGINNUMERICPROXY_H_ */
