/*
 * PluginRadioButtonProxy.cpp
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#include <iostream>

using namespace std;

#include "plugin/lua/LuaUtils.h"
#include "plugin/PluginEngine.h"
#include "plugin/api/gui/PluginTab.h"

#include "plugin/lua/proxy/gui/PluginTabProxy.h"

namespace JktPlugin {

const char* PluginTabProxy::className = "Tab";

const Luna<PluginTabProxy>::FunctionType PluginTabProxy::methods[] = {
		{"addCheckbox", &AbstractPluginPanelProxy::addCheckbox},
		{"addNotebook", &AbstractPluginPanelProxy::addNotebook},
		{"addBoxHoriz", &AbstractPluginPanelProxy::addBoxHoriz},
		{"addBoxVert", &AbstractPluginPanelProxy::addBoxVert},
		{"addNumeric", &AbstractPluginPanelProxy::addNumeric},
		{0}
};

const Luna<PluginTabProxy>::PropertyType PluginTabProxy::properties[] = {
	{0}
};

PluginTabProxy::PluginTabProxy(lua_State* L) : AbstractPluginPanelProxy(L) {
	_pluginContext = NULL;
	_pluginTab = NULL;

	LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 0);
	AbstractPluginPanelProxy::setPluginContext(PluginEngine::getPluginContext(L));

	cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur Lua : Cannot create new PluginWindowProxy in Lua";
}

PluginTabProxy::PluginTabProxy(PluginContext* pluginContext, PluginTab* pluginTab) : AbstractPluginPanelProxy(pluginContext) {
	_pluginContext = NULL;
	_pluginTab = pluginTab;

	AbstractPluginPanelProxy::setPanel(_pluginTab);
}

PluginTabProxy::~PluginTabProxy() {
}

} /* namespace JktPlugin */
