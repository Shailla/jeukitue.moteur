/*
 * PluginWindowProxy.cpp
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#include <iostream>

using namespace std;

#include "plugin/lua/LuaUtils.h"
#include "plugin/PluginEngine.h"
#include "plugin/api/gui/PluginNotebook.h"
#include "plugin/lua/proxy/gui/PluginTabProxy.h"

#include "plugin/lua/proxy/gui/PluginNotebookProxy.h"

namespace jkt {

const char PluginNotebookProxy::className[] = "Notebook";

Lunar<PluginNotebookProxy>::RegType PluginNotebookProxy::methods[] = {
		{"addTab", &PluginNotebookProxy::addTab},
		{0}
};

PluginNotebookProxy::PluginNotebookProxy(lua_State* L) {
	_pluginContext = NULL;
	_pluginNotebook = NULL;

	LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 0);

	cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur Lua : Cannot create new PluginWindowProxy in Lua";
}

PluginNotebookProxy::PluginNotebookProxy(PluginContext* pluginContext, PluginNotebook* pluginNotebook) {
	_pluginContext = pluginContext;
	_pluginNotebook = pluginNotebook;
}

PluginNotebookProxy::~PluginNotebookProxy() {
}

int PluginNotebookProxy::push(lua_State* L) {
	Lunar<PluginNotebookProxy>::push(L, this);
	return 1;
}

/**
 * Ajoute un onglet au notebook.
 *    - Param 1 : Titre de l'onglet
 */
int PluginNotebookProxy::addTab(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 1, LUA_PARAM_STRING)) {
		const string tabTitle = lua_tostring(L, 1);
		PluginTabProxy* tabProxy = new PluginTabProxy(_pluginContext, _pluginNotebook->addTab(tabTitle));
		Lunar<PluginTabProxy>::push(L, tabProxy);
		return 1;
	}

	return 0;
}

} /* namespace jkt */
