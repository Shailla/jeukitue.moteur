/*
 * PluginRadioButtonProxy.cpp
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#include <iostream>

using namespace std;

#include "main/Fabrique.h"
#include "plugin/lua/LuaUtils.h"
#include "plugin/PluginEngine.h"

#include "plugin/lua/proxy/gui/PluginBoxProxy.h"

namespace jkt {

const char PluginBoxProxy::className[] = "Box";

Lunar<PluginBoxProxy>::RegType PluginBoxProxy::methods[] = {
		// Widgets enfants
		{"addButton", &AbstractPluginPanelProxy::addButton},
		{"addCheckbox", &AbstractPluginPanelProxy::addCheckbox},
		{"addComboList", &AbstractPluginPanelProxy::addComboList},
		{"addLabel", &AbstractPluginPanelProxy::addLabel},
		{"addNumeric", &AbstractPluginPanelProxy::addNumeric},
		{"addSeparator", &AbstractPluginPanelProxy::addSeparator},

		// Containers enfants
		{"addNotebook", &AbstractPluginPanelProxy::addNotebook},
		{"addBoxHoriz", &AbstractPluginPanelProxy::addBoxHoriz},
		{"addBoxVert", &AbstractPluginPanelProxy::addBoxVert},

		// Opérations
		{"expand", &AbstractPluginPanelProxy::expand},
		{"expandHoriz", &AbstractPluginPanelProxy::expandHoriz},
		{"expandVert", &AbstractPluginPanelProxy::expandVert},
		{0}
};

PluginBoxProxy::PluginBoxProxy(lua_State* L) : AbstractPluginPanelProxy(L) {
	_pluginBox = NULL;

	LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 0);
	AbstractPluginPanelProxy::setPluginContext(Fabrique::getPluginEngine()->getGlobalPluginContext(L));

	cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur Lua : Cannot create new PluginWindowProxy in Lua";
}

PluginBoxProxy::PluginBoxProxy(PluginContext* pluginContext, PluginBox* pluginBox) : AbstractPluginPanelProxy(pluginContext) {
	_pluginBox = pluginBox;
	AbstractPluginPanelProxy::setPanel(_pluginBox);
}

PluginBoxProxy::~PluginBoxProxy() {
}

int PluginBoxProxy::push(lua_State* L) {
	Lunar<PluginBoxProxy>::push(L, this);
	return 1;
}

} /* namespace jkt */
