/*
 * PluginEngine.cpp
 *
 *  Created on: 1 mai 2012
 *      Author: Erwin
 */

#include <iostream>

using namespace std;

#include "plugin/lua/proxy/cfg/PluginConfigurationProxy.h"
#include "plugin/lua/proxy/gui/PluginComboListProxy.h"
#include "plugin/lua/proxy/gui/PluginBoxProxy.h"
#include "plugin/lua/proxy/gui/PluginButtonProxy.h"
#include "plugin/lua/proxy/gui/PluginCheckboxProxy.h"
#include "plugin/lua/proxy/gui/PluginNotebookProxy.h"
#include "plugin/lua/proxy/gui/PluginNumericProxy.h"
#include "plugin/lua/proxy/gui/PluginTabProxy.h"
#include "plugin/lua/proxy/gui/PluginWindowProxy.h"
#include "plugin/lua/LuaUtils.h"
#include "plugin/lua/LuaGlobalMethods.h"

using namespace JktPlugin;

#include "plugin/PluginEngine.h"

extern const char* PLUGINS_DIRECTORY;

namespace JktPlugin {

std::map<string, PluginContext*> PluginEngine::_mapNamePlugin;
std::map<const lua_State*, PluginContext*> PluginEngine::_mapLuaContext;

PluginEngine::PluginEngine() {
}

PluginEngine::~PluginEngine() {
}

PluginContext* PluginEngine::getPluginContext(string& pluginName) {
	return _mapNamePlugin[pluginName];
}

PluginContext* PluginEngine::getPluginContext(const lua_State* L) {
	return _mapLuaContext[L];
}

/**
 * Activate the plugin.
 */
void PluginEngine::activatePlugin(string& pluginName) {
	PluginContext* pluginContext = getPluginContext(pluginName);

	if(pluginContext != NULL) {
		cerr << endl << __FILE__ << ":" << __LINE__ << " Le plugin est déjà actif '" << pluginName << "'";
		pluginContext->logError("Tentative d'activation du plugin alors qu'il est déjà actif");
		return;
	}


	/* ******************************************************************************
	 * Initialisation des variables
	 * *****************************************************************************/

	string pluginDirectory = string(PLUGINS_DIRECTORY).append(pluginName).append("/");
	cout << endl << __FILE__ << ":" << __LINE__ << " Activation of plugin : '" << pluginName << "' in '" << pluginDirectory << "'";


	/* ******************************************************************************
	 * Initialisation Lua
	 * *****************************************************************************/

	lua_State* L = luaL_newstate();
	luaopen_io(L); 		// provides io.*
	luaopen_base(L);
	luaopen_table(L);
	luaopen_string(L);
	luaopen_math(L);
//	luaopen_loadlib(L);


	/* ******************************************************************************
	 * Création du contexte et logger du plugin
	 * *****************************************************************************/

	pluginContext = new PluginContext(L, pluginName, pluginDirectory);


	/* ******************************************************************************
	 * Initialisation du contexte du plugin
	 * *****************************************************************************/

	pluginContext->logInfo("Consolidation du contexte...");

	_mapNamePlugin[pluginName] = pluginContext;
	_mapLuaContext[L] = pluginContext;


	/* *******************************************************************************
	 * Déclaration des fonctions et classes mises à disposition dans les plugins
	 * ******************************************************************************/

	// Initialisation des fonctions dans Lua
	pluginContext->logInfo("Initialisation des fonctions Lua...");

	lua_register(L, "getScreenSize", &PluginConfigurationProxy::getScreenSize);

	lua_register(L, "getAvailableAudioDrivers", &PluginConfigurationProxy::getAvailableAudioDrivers);
	lua_register(L, "getConfigurationParameter", &PluginConfigurationProxy::getConfigurationParameter);
	lua_register(L, "setConfigurationParameter", &PluginConfigurationProxy::setConfigurationParameter);

	lua_register(L, "log", &LuaGlobalMethods::log);

	// Initialisation des classes dans Lua
	pluginContext->logInfo("Initialisation des classes Lua...");

	Luna<PluginCheckboxProxy>::Register(L);
	Luna<PluginComboListProxy>::Register(L);
	Luna<PluginButtonProxy>::Register(L);
	Luna<PluginBoxProxy>::Register(L);
	Luna<PluginNotebookProxy>::Register(L);
	Luna<PluginNumericProxy>::Register(L);
	Luna<PluginTabProxy>::Register(L);
	Luna<PluginWindowProxy>::Register(L);


	/* ******************************************************************************
	 * Chargement du fichier Lua du plugin
	 * *****************************************************************************/

	string pluginFile = string(pluginDirectory).append(pluginName).append(".lua");
	pluginContext->logInfo(string("Chargement du script '").append(pluginFile).append("'..."));

	int status = luaL_loadfile(L, pluginFile.c_str());

	if(status != 0) {
		pluginContext->logScriptError("Echec de chargement du fichier du plugin");
		pluginContext->logLuaError(status);
		pluginContext->logError("Echec d'initialisation du plugin.");
		lua_close(L);
		delete pluginContext;

		return;
	}


	/* *******************************************************************************
	 * Initialisation du script
	 * ******************************************************************************/

	pluginContext->logInfo("Initialisation du script...");

	// Appel principal pour l'initialisation du script chargé
	status = lua_pcall(L, 0, LUA_MULTRET, 0);

	if(status != 0) {
		pluginContext->logLuaError(status);
		pluginContext->logError("Echec d'initialisation du plugin.");
		lua_close(L);

		_mapNamePlugin.erase(pluginName);
		_mapLuaContext.erase(L);

		delete pluginContext;

		return;
	}


	/* *******************************************************************************
	 * Exécution de la méthode d'initialisation du plugin
	 * ******************************************************************************/

	pluginContext->logInfo("Exécution de la fonction onLoad du script...");

	lua_getglobal(L, "onLoad");

	// Vérifie si la fonction d'initialisation du plugin existe bien
	if(!lua_isfunction(L, -1)) {
		// la fonction n'existe pas
		lua_pop(L, 1);

		pluginContext->logError("La fonction 'onLoad' n'existe pas, tout plugin doit définir cette fonction.");
		pluginContext->logLuaError(status);
		pluginContext->logError("Echec d'initialisation du plugin.");
		lua_close(L);

		_mapNamePlugin.erase(pluginName);
		_mapLuaContext.erase(L);

		delete pluginContext;

		return;
	}

	// Exécution de la méthode d'init du plugin
	status = lua_pcall(L, 0, 0, 0);

	if(status) {
		pluginContext->logLuaError(status);
		pluginContext->logError("Echec d'initialisation du plugin.");
		lua_close(L);

		_mapNamePlugin.erase(pluginName);
		_mapLuaContext.erase(L);

		delete pluginContext;

		return;
	}

	pluginContext->logInfo("Plugin activé.");
}

/**
 * Deactivate the plugin.
 */
void PluginEngine::deactivatePlugin(string& pluginName) {
	PluginContext* pluginContext = getPluginContext(pluginName);

	if(pluginContext != NULL) {
		pluginContext->logInfo("Début de désactivation du plugin");

		lua_close(pluginContext->getLuaState());

		_mapLuaContext.erase(pluginContext->getLuaState());
		_mapNamePlugin.erase(pluginName);

		pluginContext->logInfo("Plugin désactivé");
		delete pluginContext;
	}
	else {
		cerr << endl << __FILE__ << ":" << __LINE__ << " Le plugin ne peut pas être désactivé, il n'est pas actif '" << pluginName << "'";
	}
}

} /* namespace JktPlugin */
