/*
 * PluginNumericProxy.cpp
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#include <iostream>

using namespace std;

#include "plugin/lua/LuaUtils.h"
#include "plugin/PluginEngine.h"

#include "plugin/lua/proxy/gui/PluginNumericProxy.h"

namespace JktPlugin {

const char PluginNumericProxy::className[] = "Numeric";

Lunar<PluginNumericProxy>::RegType PluginNumericProxy::methods[] = {
		{"getValue", &PluginNumericProxy::getValue},
		{0, 0}
};

PluginNumericProxy::PluginNumericProxy(lua_State* L) {
	LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 0);

	cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur Lua : Cannot create new PluginNumericProxy in Lua";
}

PluginNumericProxy::PluginNumericProxy(PluginNumeric* pluginNumeric) {
	_pluginNumeric = pluginNumeric;
}

PluginNumericProxy::~PluginNumericProxy() {
}

/**
 * Modifier les dimensions de la fenêtre.
 *    - Return 1 : Value setted in the numeric field
 */
int PluginNumericProxy::getValue(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 0)) {
		float value = _pluginNumeric->getValue();
		lua_pushnumber(L, value);
	}

	return 1;
}

} /* namespace JktPlugin */
