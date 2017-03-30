/*
 * LuaGlobalMethods.cpp
 *
 *  Created on: 20 mai 2012
 *      Author: Erwin
 */

#include "util/Trace.h"
#include "main/Fabrique.h"
#include "menu/Controller.h"
#include "plugin/PluginEngine.h"
#include "plugin/lua/LuaUtils.h"
#include "menu/ConsoleView.h"

#include "plugin/lua/LuaGlobalMethods.h"

using namespace std;

namespace jkt {

const char* LuaGlobalMethods::REFRESH_EVENT_TYPE = "refresh";

LuaGlobalMethods::LuaGlobalMethods() {
}

LuaGlobalMethods::~LuaGlobalMethods() {
}

int LuaGlobalMethods::log(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 1, LUA_PARAM_STRING)) {
		PluginContext* pluginContext = Fabrique::getPluginEngine()->getGlobalPluginContext(L);
		string msg = lua_tostring(L, 1);

		if(pluginContext) {		// Recherche dans les plugins globaux
			pluginContext->logUser(msg);
		}
		else {					// Sinon recherche dans les plugins de la Map
			pluginContext = Fabrique::getPluginEngine()->getMapPluginContext(L);

			if(pluginContext) {
				pluginContext->logUser(msg);
			}
			else {
				LOGERROR(("Contexte de plugin introuvable"));
			}
		}
	}

	return 0;
}

/**
 * Send a new event from the plugin to the JKT event controller
 */
int LuaGlobalMethods::pushEvent(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 1, LUA_PARAM_NUMBER)) {
		int pluginActionId = lua_tonumber(L, 1);

		if(pluginActionId < Controller::Action::PLUGIN_BASE_ID) {
			PluginContext* pluginContext = Fabrique::getPluginEngine()->getGlobalPluginContext(L);
			string message = "Illegal use of an internal action id (";
			message += pluginActionId;
			message += ")";
			pluginContext->logError(message);
		}
		else {
			Controller::executeAction(pluginActionId);
		}
	}

	return 0;
}

/**
 * Subscribe the plugin to receive events of a specific type
 */
int LuaGlobalMethods::subscribeEvents(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 1, LUA_PARAM_STRING)) {
		PluginContext* pluginContext = Fabrique::getPluginEngine()->getPluginContext(L);
		string type = lua_tostring(L, 1);

		if(type == REFRESH_EVENT_TYPE) {
			pluginContext->subscribeRefreshEvents(true);
		}
		else {
			pluginContext->logError("Unknown event subscription type : '" + type + "'");
		}
	}

	return 0;
}

/**
 */
int LuaGlobalMethods::logConsoleInfo(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 1, LUA_PARAM_STRING)) {
		const string msg = lua_tostring(L, 1);

		ConsoleView* console = ((ConsoleView*)Fabrique::getAgarView()->getView(Viewer::CONSOLE_VIEW));
		console->println(ConsoleView::ConsoleOutputType::COT_INFO, msg);

		return 0;
	}

	return 0;
}


} /* namespace jkt */
