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
#include "plugin/api/gui/PluginTab.h"

#include "plugin/lua/proxy/gui/PluginTabProxy.h"

namespace jkt {

const char PluginTabProxy::className[] = "Tab";

Lunar<PluginTabProxy>::RegType PluginTabProxy::methods[] = {
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

PluginTabProxy::PluginTabProxy(lua_State* L) : AbstractPluginPanelProxy(L) {
	_pluginContext = NULL;
	_pluginTab = NULL;

	LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 0);
	AbstractPluginPanelProxy::setPluginContext(Fabrique::getPluginEngine()->getGlobalPluginContext(L));

	cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur Lua : Cannot create new PluginWindowProxy in Lua";
}

PluginTabProxy::PluginTabProxy(PluginContext* pluginContext, PluginTab* pluginTab) : AbstractPluginPanelProxy(pluginContext) {
	_pluginContext = NULL;
	_pluginTab = pluginTab;

	AbstractPluginPanelProxy::setPanel(_pluginTab);
}

PluginTabProxy::~PluginTabProxy() {
}

int PluginTabProxy::push(lua_State* L) {
	Lunar<PluginTabProxy>::push(L, this);
	return 1;
}

} /* namespace jkt */
