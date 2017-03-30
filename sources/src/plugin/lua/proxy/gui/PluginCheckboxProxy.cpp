/*
 * PluginRadioButtonProxy.cpp
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#include <iostream>

#include "plugin/lua/LuaUtils.h"
#include "plugin/PluginEngine.h"

#include "plugin/lua/proxy/gui/PluginCheckboxProxy.h"

using namespace std;

namespace jkt {

const char PluginCheckboxProxy::className[] = "Checkbox";

Lunar<PluginCheckboxProxy>::RegType PluginCheckboxProxy::methods[] = {
		{"getValue", &PluginCheckboxProxy::getValue},
		{"setValue", &PluginCheckboxProxy::setValue},
		{0}
};

PluginCheckboxProxy::PluginCheckboxProxy(lua_State* L) {
	_pluginCheckbox = NULL;

	LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 0);

	cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur Lua : Cannot create new PluginCheckboxProxy in Lua";
}

PluginCheckboxProxy::PluginCheckboxProxy(PluginCheckbox* pluginCheckbox) {
	_pluginCheckbox = pluginCheckbox;
}

PluginCheckboxProxy::~PluginCheckboxProxy() {
}

int PluginCheckboxProxy::push(lua_State* L) {
	Lunar<PluginCheckboxProxy>::push(L, this);
	return 1;
}

int PluginCheckboxProxy::getValue(lua_State* L) {
	LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 0);
	const bool value = _pluginCheckbox->getValue();
	lua_pushboolean(L, value);

	return 1;
}

int PluginCheckboxProxy::setValue(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 1, LUA_PARAM_BOOLEAN)) {
		const bool value = lua_toboolean(L, 1);
		_pluginCheckbox->setValue(value);
	}

	return 0;
}

} /* namespace jkt */
