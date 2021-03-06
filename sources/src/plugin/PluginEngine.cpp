/*
 * PluginEngine.cpp
 *
 *  Created on: 1 mai 2012
 *      Author: Erwin
 */

#include <iostream>

using namespace std;

#include "plugin/lua/proxy/PluginEventProxy.h"
#include "plugin/lua/proxy/cfg/PluginConfigurationProxy.h"
#include "plugin/lua/proxy/gui/PluginComboListProxy.h"
#include "plugin/lua/proxy/gui/PluginBoxProxy.h"
#include "plugin/lua/proxy/gui/PluginButtonProxy.h"
#include "plugin/lua/proxy/gui/PluginLabelProxy.h"
#include "plugin/lua/proxy/gui/PluginCheckboxProxy.h"
#include "plugin/lua/proxy/gui/PluginNotebookProxy.h"
#include "plugin/lua/proxy/gui/PluginNumericProxy.h"
#include "plugin/lua/proxy/gui/PluginTabProxy.h"
#include "plugin/lua/proxy/gui/PluginWindowProxy.h"
#include "plugin/lua/LuaUtils.h"
#include "plugin/lua/LuaGlobalMethods.h"
#include "main/Cfg.h"

#include "plugin/PluginEngine.h"

extern const char* PLUGINS_DIRECTORY;
extern CCfg Config;

namespace JktPlugin {

const string PluginEngine::PLUGIN_MAIN_FILENAME = "main";

PluginEngine::PluginEngine() {
}

PluginEngine::~PluginEngine() {
}

void PluginEngine::dispatchEvent(const PluginActionEvent& event) {
	std::map<std::string, PluginContext*>::iterator iter = _nameGlobalPlugin.begin();

	for( ; iter != _nameGlobalPlugin.end() ; iter++) {
		iter->second->dispatchEvent(event);
	}
}

PluginContext* PluginEngine::getGlobalPluginContext(const string& pluginName) {
	std::map<std::string, PluginContext*>::iterator iter = _nameGlobalPlugin.find(pluginName);

	if(iter == _nameGlobalPlugin.end()) {
		return 0;
	}
	else {
		return iter->second;
	}
}

PluginContext* PluginEngine::getMapPluginContext(const string& pluginName) {
	std::map<std::string, PluginContext*>::iterator iter = _nameMapPlugin.find(pluginName);

	if(iter == _nameMapPlugin.end()) {
		return 0;
	}
	else {
		return iter->second;
	}
}

PluginContext* PluginEngine::getGlobalPluginContext(const lua_State* L) {
	std::map<const lua_State*, PluginContext*>::iterator iter = _luaGlobalContext.find(L);

	if(iter == _luaGlobalContext.end()) {
		return 0;
	}
	else {
		return iter->second;
	}
}

PluginContext* PluginEngine::getMapPluginContext(const lua_State* L) {
	std::map<const lua_State*, PluginContext*>::iterator iter = _luaMapContext.find(L);

	if(iter == _luaMapContext.end()) {
		return 0;
	}
	else {
		return iter->second;
	}
}

/**
 * Activate the some plugins which are activated by default, the list of them
 * is given in the Jkt configuration.
 */
void PluginEngine::activateDefaultGlobalPlugins() {
	vector<string>::iterator iter;

	cout << endl << "ACTIVATION DES PLUGINS PAR DEFAUT";

	for(iter = Config.Plugin._defaultPluging.begin() ; iter !=Config.Plugin._defaultPluging.end() ; iter++) {
		activateGlobalPlugin(*iter);
	}
}

PluginContext* PluginEngine::activatePlugin(const string& pluginName, const string& pluginDirectory) {

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
	 * Cr�ation du contexte et logger du plugin
	 * *****************************************************************************/

	PluginContext* pluginContext = new PluginContext(L, pluginName, pluginDirectory);


	/* *******************************************************************************
	 * D�claration des fonctions et classes mises � disposition dans les plugins
	 * ******************************************************************************/

	pluginContext->logInfo("Consolidation du contexte...");

	// Initialisation des fonctions dans Lua
	pluginContext->logInfo("Initialisation des fonctions Lua...");

	lua_register(L, "log", &LuaGlobalMethods::log);
	lua_register(L, "pushEvent", &LuaGlobalMethods::pushEvent);

	lua_register(L, "getScreenSize", &PluginConfigurationProxy::getScreenSize);
	lua_register(L, "getAvailableAudioDrivers", &PluginConfigurationProxy::getAvailableAudioDrivers);
	lua_register(L, "getAvailableAudioRecordDrivers", &PluginConfigurationProxy::getAvailableAudioRecordDrivers);
	lua_register(L, "getAvailableAudioOutputs", &PluginConfigurationProxy::getAvailableAudioOutputs);
	lua_register(L, "getConfigurationParameter", &PluginConfigurationProxy::getConfigurationParameter);
	lua_register(L, "setConfigurationParameter", &PluginConfigurationProxy::setConfigurationParameter);
	lua_register(L, "getConstant", &PluginConfigurationProxy::getConstant);
	lua_register(L, "initAudio", &PluginConfigurationProxy::initAudio);


	// Initialisation des classes dans Lua
	pluginContext->logInfo("Initialisation des classes Lua...");

	Lunar<PluginEventProxy>::Register(L);
	Lunar<PluginButtonProxy>::Register(L);
	Lunar<PluginCheckboxProxy>::Register(L);
	Lunar<PluginComboListProxy>::Register(L);
	Lunar<PluginLabelProxy>::Register(L);
	Lunar<PluginNumericProxy>::Register(L);
	Lunar<PluginBoxProxy>::Register(L);
	Lunar<PluginNotebookProxy>::Register(L);
	Lunar<PluginTabProxy>::Register(L);
	Lunar<PluginWindowProxy>::Register(L);


	/* ******************************************************************************
	 * Chargement du fichier Lua du plugin
	 * *****************************************************************************/

	string pluginFile = string(pluginDirectory).append(pluginName).append(".lua");
	pluginContext->logInfo(string("Chargement du script '").append(pluginFile).append("'..."));

	int status = luaL_loadfile(L, pluginFile.c_str());

	if(status != 0) {
		pluginContext->logError("Echec de chargement du fichier du plugin");
		pluginContext->logLuaError(status);
		pluginContext->logError("Echec d'initialisation du plugin.");

		lua_close(L);
		delete pluginContext;

		return 0;
	}

	/* *******************************************************************************
	 * Initialisation du script
	 * ******************************************************************************/

	pluginContext->logInfo("Initialisation du script...");

	// Appel principal pour l'initialisation du script charg�
	status = lua_pcall(L, 0, LUA_MULTRET, 0);

	if(status != 0) {
		pluginContext->logError("Echec d'initialisation du script");
		pluginContext->logLuaError(status);
		pluginContext->logError("Echec d'initialisation du plugin.");

		lua_close(L);
		delete pluginContext;

		return 0;
	}

	/* *******************************************************************************
	 * V�rification de la pr�sence d'une fonction "eventManager" dans le plugin
	 * ******************************************************************************/

	lua_getglobal(L, "eventManager");

	if (!lua_isfunction(L, -1)) {
		// la fonction n'existe pas
		pluginContext->logInfo("la fonction 'eventManager' n'existe pas");
	}

	lua_pop(L, 1);


	/* *******************************************************************************
	 * Ex�cution de la m�thode d'initialisation du plugin ("onLoad")
	 * ******************************************************************************/

	pluginContext->logInfo("Ex�cution de la fonction onLoad du script...");

	lua_getglobal(L, "onLoad");

	// V�rifie si la fonction d'initialisation du plugin existe bien
	if(!lua_isfunction(L, -1)) {
		// la fonction n'existe pas
		lua_pop(L, 1);

		pluginContext->logError("La fonction 'onLoad' n'existe pas, tout plugin doit d�finir cette fonction.");
		pluginContext->logLuaError(status);
		pluginContext->logError("Echec d'initialisation du plugin.");

		lua_close(L);
		delete pluginContext;

		return 0;
	}

	return pluginContext;
}

/**
 * Activate the plugin.
 */
void PluginEngine::activateGlobalPlugin(const string& pluginName) {
	PluginContext* pluginContext = getGlobalPluginContext(pluginName);

	if(pluginContext != NULL) {
		cerr << endl << __FILE__ << ":" << __LINE__ << " Le plugin est d�j� actif '" << pluginName << "'";
		pluginContext->logError("Tentative d'activation du plugin alors qu'il est d�j� actif");
		return;
	}

	/* ******************************************************************************
	 * Initialisation des variables
	 * *****************************************************************************/

	const string pluginDirectory = string(PLUGINS_DIRECTORY).append(pluginName).append("/");
	cout << endl << __FILE__ << ":" << __LINE__ << " Activation of plugin : '" << pluginName << "' in '" << pluginDirectory << "'";

	pluginContext = activatePlugin("main", pluginDirectory);

	if(pluginContext) {
		// R�f�rencement du plugin
		lua_State* L = pluginContext->getLuaState();

		_nameGlobalPlugin[pluginName] = pluginContext;
		_luaGlobalContext[L] = pluginContext;

		pluginContext->logInfo("Plugin initialis�");

		// Ex�cution de la m�thode d'init du plugin
		pluginContext->logInfo("Ex�cution du plugin");

		int status = lua_pcall(L, 0, 0, 0);

		if(status) {
			pluginContext->logLuaError(status);
			pluginContext->logError("Echec d'ex�cution du plugin.");
		}
		else {
			pluginContext->logInfo("Plugin en cours d'ex�cution");
		}
	}
}

/**
 * Activate the plugin.
 */
void PluginEngine::activateMapPlugin(const string& pluginName, const string pluginDirectory) {
	PluginContext* pluginContext = getMapPluginContext(pluginName);

	if(pluginContext != NULL) {
		cerr << endl << __FILE__ << ":" << __LINE__ << "Le plugin de Map est d�j� actif '" << pluginName << "'";
		pluginContext->logError("Tentative d'activation du plugin de Map alors qu'il est d�j� actif");
		return;
	}

	/* ******************************************************************************
	 * Initialisation des variables
	 * *****************************************************************************/

	cout << endl << __FILE__ << ":" << __LINE__ << " Activation of Map plugin : '" << pluginName << "' in '" << pluginDirectory << "'";

	pluginContext = activatePlugin(pluginName, pluginDirectory);

	if(pluginContext) {
		// R�f�rencement du plugin
		lua_State* L = pluginContext->getLuaState();

		_nameMapPlugin[pluginName] = pluginContext;
		_luaMapContext[L] = pluginContext;

		pluginContext->logInfo("Plugin de Map initialis�");

		// Ex�cution de la m�thode d'init du plugin
		pluginContext->logInfo("Ex�cution du plugin de Map");

		int status = lua_pcall(L, 0, 0, 0);

		if(status) {
			pluginContext->logLuaError(status);
			pluginContext->logError("Echec d'ex�cution du plugin de Map.");
		}
		else {
			pluginContext->logInfo("Plugin de Map en cours d'ex�cution");
		}
	}
}

void PluginEngine::deactivateMapPlugins() {
	map<std::string, PluginContext*>::iterator itName;
	map<const lua_State*, PluginContext*>::iterator itContext;

	for(itName = _nameMapPlugin.begin() ; itName != _nameMapPlugin.end() ; itName++) {
		PluginContext* pluginContext = itName->second;

		if(pluginContext != NULL) {
			pluginContext->logInfo("D�but de d�sactivation du plugin de Map");

			lua_close(pluginContext->getLuaState());
			pluginContext->logInfo("Plugin de Map d�sactiv�");

			delete pluginContext;
		}
		else {
			cerr << endl << __FILE__ << ":" << __LINE__ << " Le plugin de Map ne peut pas �tre d�sactiv�, il n'est pas actif '" << itName->first << "'";
		}
	}

	_nameMapPlugin.clear();
	_luaMapContext.clear();
}


/**
 * Deactivate the plugin.
 */
void PluginEngine::deactivateGlobalPlugin(const string& pluginName) {
	PluginContext* pluginContext = getGlobalPluginContext(pluginName);

	if(pluginContext != NULL) {
		pluginContext->logInfo("D�but de d�sactivation du plugin");

		lua_close(pluginContext->getLuaState());

		_luaGlobalContext.erase(pluginContext->getLuaState());
		_nameGlobalPlugin.erase(pluginName);

		pluginContext->logInfo("Plugin d�sactiv�");

		delete pluginContext;
	}
	else {
		cerr << endl << __FILE__ << ":" << __LINE__ << " Le plugin ne peut pas �tre d�sactiv�, il n'est pas actif '" << pluginName << "'";
	}
}

} /* namespace JktPlugin */
