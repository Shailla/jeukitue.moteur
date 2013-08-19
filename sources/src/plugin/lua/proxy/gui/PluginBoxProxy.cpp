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

#include "plugin/lua/proxy/gui/PluginBoxProxy.h"

namespace JktPlugin {

const char* PluginBoxProxy::className = "Box";

const Luna<PluginBoxProxy>::FunctionType PluginBoxProxy::methods[] = {
		{"addButton", &AbstractPluginPanelProxy::addButton},
		{"addCheckbox", &AbstractPluginPanelProxy::addCheckbox},
		{"addNotebook", &AbstractPluginPanelProxy::addNotebook},
		{"addBoxHoriz", &AbstractPluginPanelProxy::addBoxHoriz},
		{"addBoxVert", &AbstractPluginPanelProxy::addBoxVert},
		{"addNumeric", &AbstractPluginPanelProxy::addNumeric},
		{0}
};

const Luna<PluginBoxProxy>::PropertyType PluginBoxProxy::properties[] = {
	{0}
};

PluginBoxProxy::PluginBoxProxy(lua_State* L) : AbstractPluginPanelProxy(L) {
	_pluginBox = NULL;

	LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 0);
	AbstractPluginPanelProxy::setPluginContext(PluginEngine::getPluginContext(L));

	cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur Lua : Cannot create new PluginWindowProxy in Lua";
}

PluginBoxProxy::PluginBoxProxy(PluginContext* pluginContext, PluginBox* pluginBox) : AbstractPluginPanelProxy(pluginContext) {
	_pluginBox = pluginBox;
	AbstractPluginPanelProxy::setPanel(_pluginBox);
}

PluginBoxProxy::~PluginBoxProxy() {
}

} /* namespace JktPlugin */
