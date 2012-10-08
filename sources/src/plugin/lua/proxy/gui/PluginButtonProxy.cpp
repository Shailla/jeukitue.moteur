/*
 * PluginButtonProxy.cpp
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#include <iostream>

using namespace std;

#include "plugin/lua/LuaUtils.h"
#include "plugin/PluginEngine.h"

#include "plugin/lua/proxy/gui/PluginButtonProxy.h"

namespace JktPlugin {

const char PluginButtonProxy::className[] = "Button";

Lunar<PluginButtonProxy>::RegType PluginButtonProxy::methods[] = {
		{0, 0}
};

PluginButtonProxy::PluginButtonProxy(lua_State* L) {
	_pluginContext = NULL;
	_pluginButton = NULL;
}

PluginButtonProxy::PluginButtonProxy(PluginContext* pluginContext) {
	_pluginContext = pluginContext;
	_pluginButton = NULL;
}

PluginButtonProxy::~PluginButtonProxy() {
}

void PluginButtonProxy::setWrappedObject(PluginButton* pluginButton) {
	_pluginButton = pluginButton;
}

void PluginButtonProxy::buttonPressedEvent(void) {
	lua_getglobal(_pluginContext->getLuaState(), "eventManager");

	// on vérifie si la fonction existe bien
	if (!lua_isfunction(_pluginContext->getLuaState(), -1)) {
		// la fonction n'existe pas
		_pluginContext->logError("la fonction eventManager n'existe pas");
		lua_pop(_pluginContext->getLuaState(), 1);
	}
	else {
		Lunar<PluginButtonProxy>::push(_pluginContext->getLuaState(), this, false);
		lua_call(_pluginContext->getLuaState(), 1, 0);
	}
}

} /* namespace JktPlugin */
