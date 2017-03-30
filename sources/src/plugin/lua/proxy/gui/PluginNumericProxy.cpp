/*
 * PluginNumericProxy.cpp
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#include <iostream>

#include "plugin/lua/LuaUtils.h"
#include "plugin/PluginEngine.h"

#include "plugin/lua/proxy/gui/PluginNumericProxy.h"

using namespace std;

namespace jkt {

const char PluginNumericProxy::className[] = "Numeric";

Lunar<PluginNumericProxy>::RegType PluginNumericProxy::methods[] = {
		{"getValue", &PluginNumericProxy::getValue},
		{0}
};

PluginNumericProxy::PluginNumericProxy(lua_State* L) {
	_pluginNumeric = NULL;

	LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 0);

	cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur Lua : Cannot create new PluginNumericProxy in Lua";
}

PluginNumericProxy::PluginNumericProxy(PluginNumeric* pluginNumeric) {
	_pluginNumeric = pluginNumeric;
}

PluginNumericProxy::~PluginNumericProxy() {
}

int PluginNumericProxy::push(lua_State* L) {
	Lunar<PluginNumericProxy>::push(L, this);
	return 1;
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

} /* namespace jkt */
