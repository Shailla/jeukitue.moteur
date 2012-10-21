/*
 * PluginConfigurationProxy.cpp
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#include <iostream>
#include <sstream>

using namespace std;

#include "plugin/lua/LuaUtils.h"
#include "plugin/PluginEngine.h"
#include "main/Cfg.h"

#include "plugin/lua/proxy/cfg/PluginConfigurationProxy.h"

extern CCfg Config;		// Contient la configuration du jeu

namespace JktPlugin {

const char* PluginConfigurationProxy::CFG_PARAM_SKIN_VISIBILITY = "SKIN_VISIBILITY";						// Paramètre de type booléen
const char* PluginConfigurationProxy::CFG_PARAM_PLAYER_OUTLINE_VISIBILITY = "PLAYER_OUTLINE_VISIBILITY";	// Paramètre de type booléen
const char* PluginConfigurationProxy::CFG_PARAM_CUBIC_METER_VISIBILITY = "CUBIC_METER_VISIBILITY";			// Paramètre de type booléen
const char* PluginConfigurationProxy::CFG_PARAM_AXES_METER_VISIBILITY = "AXES_METER_VISIBILITY";			// Paramètre de type booléen

/**
 * Obtenir les dimensions de l'écran.
 *    - Return 1 : Dimension horizontale en pixels
 *    - Return 2 : Dimension verticale en pixels
 */
int PluginConfigurationProxy::getScreenSize(lua_State *L) {
	LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 0);

	lua_pushnumber(L, 123);
	lua_pushnumber(L, 123);

	return 2;
}


/**
 * Retourne la valeur d'un paramètre global de configuration du moteur.
 *    - Param 1 : nom du paramètre demandé
 *    - Return 1 : valeur du paramètre
 */
int PluginConfigurationProxy::getConfigurationParameter(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 1, LUA_PARAM_STRING)) {
		const string paramName = lua_tostring(L, 1);

		if(paramName.compare(CFG_PARAM_SKIN_VISIBILITY) == 0) {
			lua_pushboolean(L, Config.Joueur.skinVisibility);
		}
		else if(paramName.compare(CFG_PARAM_PLAYER_OUTLINE_VISIBILITY) == 0) {
			lua_pushboolean(L, Config.Joueur.outlineVisibility);
		}
		else if(paramName.compare(CFG_PARAM_CUBIC_METER_VISIBILITY) == 0) {
			lua_pushboolean(L, Config.Debug.cubicMeterVisibility);
		}
		else if(paramName.compare(CFG_PARAM_AXES_METER_VISIBILITY) == 0) {
			lua_pushboolean(L, Config.Debug.axesMeterVisibility);
		}
		else {
			ostringstream message;
			message << "Nom du parametre de configuration inconnu (recu='" << paramName << "')";
			PluginEngine::getPluginContext(L)->logError(message.str());
		}
	}

	return 1;
}

/**
 * Change la valeur d'un paramètre global de configuration du moteur.
 *    - Param 1 : nom du paramètre demandé
 *    - Param 2 : nouvelle valeur du paramètre
 */
int PluginConfigurationProxy::setConfigurationParameter(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 2, LUA_PARAM_STRING, LUA_PARAM_ANY)) {
		const string paramName = lua_tostring(L, 1);

		if(paramName.compare(CFG_PARAM_SKIN_VISIBILITY) == 0) {
			bool value = lua_toboolean(L, 2);
			Config.Joueur.skinVisibility = value;
		}
		else if(paramName.compare(CFG_PARAM_PLAYER_OUTLINE_VISIBILITY) == 0) {
			bool value = lua_toboolean(L, 2);
			Config.Joueur.outlineVisibility = value;
		}
		else if(paramName.compare(CFG_PARAM_CUBIC_METER_VISIBILITY) == 0) {
			bool value = lua_toboolean(L, 2);
			Config.Debug.cubicMeterVisibility = value;
		}
		else if(paramName.compare(CFG_PARAM_AXES_METER_VISIBILITY) == 0) {
			bool value = lua_toboolean(L, 2);
			Config.Debug.axesMeterVisibility = value;
		}
		else {
			ostringstream message;
			message << "Nom du parametre de configuration inconnu (recu='" << paramName << "')";
			PluginEngine::getPluginContext(L)->logError(message.str());
		}
	}

	return 0;
}

} /* namespace JktPlugin */
