/*
 * PluginEngine.cpp
 *
 *  Created on: 1 mai 2012
 *      Author: Erwin
 */

#include <time.h>

#include "util/Trace.h"
#include "plugin/lua/proxy/PluginEventProxy.h"
#include "plugin/lua/proxy/cfg/PluginConfigurationProxy.h"
#include "plugin/lua/proxy/data/PluginDataTreeProxy.h"
#include "plugin/lua/proxy/data/PluginDataValeurProxy.h"
#include "plugin/lua/proxy/gui/PluginComboListProxy.h"
#include "plugin/lua/proxy/gui/PluginBoxProxy.h"
#include "plugin/lua/proxy/gui/PluginButtonProxy.h"
#include "plugin/lua/proxy/gui/PluginLabelProxy.h"
#include "plugin/lua/proxy/gui/PluginCheckboxProxy.h"
#include "plugin/lua/proxy/gui/PluginNotebookProxy.h"
#include "plugin/lua/proxy/gui/PluginNumericProxy.h"
#include "plugin/lua/proxy/gui/PluginTabProxy.h"
#include "plugin/lua/proxy/gui/PluginWindowProxy.h"
#include "plugin/lua/proxy/map/PluginMapProxy.h"
#include "plugin/lua/proxy/map/PluginPlayerZoneDetectorProxy.h"
#include "plugin/lua/proxy/game/PluginPlayerProxy.h"
#include "plugin/lua/proxy/game/PluginGameProxy.h"
#include "plugin/lua/proxy/game/PluginPlayerZoneEventProxy.h"
#include "plugin/lua/LuaUtils.h"
#include "plugin/lua/LuaGlobalMethods.h"
#include "main/Cfg.h"

#include "plugin/PluginEngine.h"

using namespace std;

extern const char* PLUGINS_DIRECTORY;
extern CCfg Config;

namespace jkt {

const string PluginEngine::PLUGIN_MAIN_FILENAME = "main";

PluginEngine::PluginEngine() {
}

PluginEngine::~PluginEngine() {
}

void PluginEngine::sendRefreshEvent() {
	// Global plugins
	{
		std::map<std::string, PluginContext*>::iterator iter = _nameGlobalPlugin.begin();

		for( ; iter != _nameGlobalPlugin.end() ; iter++) {
			PluginContext* context = iter->second;

			if(context->isSubscribedRefreshEvents()) {
				context->dispatchEvent(Controller::Action::RefreshMap);
			}
		}
	}

	// Map plugins
	{
		std::map<std::string, PluginContext*>::iterator iter = _nameMapPlugin.begin();

		for( ; iter != _nameMapPlugin.end() ; iter++) {
			PluginContext* context = iter->second;

			if(context->isSubscribedRefreshEvents()) {
				context->dispatchEvent(Controller::Action::RefreshMap);
			}
		}
	}
}

void PluginEngine::dispatchEvent(PluginEventProxy& plugEvent) {
	// Global plugins dispatching
	{
		std::map<std::string, PluginContext*>::iterator iter = _nameGlobalPlugin.begin();

		for( ; iter != _nameGlobalPlugin.end() ; iter++) {
			PluginEventProxy* evt = new PluginEventProxy(plugEvent);
			iter->second->dispatchEvent(evt);
		}
	}

	// Map plugins dispatching
	{
		std::map<std::string, PluginContext*>::iterator iter = _nameMapPlugin.begin();

		for( ; iter != _nameMapPlugin.end() ; iter++) {
			PluginEventProxy* evt = new PluginEventProxy(plugEvent);
			iter->second->dispatchEvent(evt);
		}
	}
}

void PluginEngine::dispatchEvent(int actionId) {
	// Global plugins dispatching
	{
		std::map<std::string, PluginContext*>::iterator iter = _nameGlobalPlugin.begin();

		for( ; iter != _nameGlobalPlugin.end() ; iter++) {
			iter->second->dispatchEvent(actionId);
		}
	}

	// Map plugins dispatching
	{
		std::map<std::string, PluginContext*>::iterator iter = _nameMapPlugin.begin();

		for( ; iter != _nameMapPlugin.end() ; iter++) {
			iter->second->dispatchEvent(actionId);
		}
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

PluginContext* PluginEngine::getPluginContext(const lua_State* L) {
	std::map<const lua_State*, PluginContext*>::iterator iter = _luaGlobalContext.find(L);

	if(iter == _luaGlobalContext.end()) {
		iter = _luaMapContext.find(L);

		if(iter == _luaMapContext.end()) {
			return 0;
		}
		else {
			return iter->second;
		}
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

PluginContext* PluginEngine::getGlobalPluginContext(const lua_State* L) {
	std::map<const lua_State*, PluginContext*>::iterator iter = _luaGlobalContext.find(L);

	if(iter == _luaGlobalContext.end()) {
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

	LOGINFO(("ACTIVATION DES PLUGINS PAR DEFAUT"));

	for(iter = Config.Plugin._defaultPluging.begin() ; iter !=Config.Plugin._defaultPluging.end() ; iter++) {
		activateGlobalPlugin(*iter);
	}
}

PluginContext* PluginEngine::activatePlugin(const string& pluginName, const string& pluginDirectory) {

	/* ******************************************************************************
	 * Initialisation Lua
	 * *****************************************************************************/

	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	/* ******************************************************************************
	 * Création du contexte et logger du plugin
	 * *****************************************************************************/

	PluginContext* pluginContext = new PluginContext(L, pluginName, pluginDirectory);


	/* *******************************************************************************
	 * Déclaration des fonctions et classes mises à disposition dans les plugins
	 * ******************************************************************************/

	// Log date de lancement du plugin
	time_t t = time(0);
	struct tm* now = localtime(&t);
	ostringstream date;
	date << "--- " << now->tm_mday << "/" << (now->tm_mon+1) << "/" << (now->tm_year+1900)
			<< " "
			<< now->tm_hour << ":" << now->tm_min << ":" << now->tm_sec << " ---";
	pluginContext->logInfo(date.str());

	pluginContext->logInfo("Consolidation du contexte...");

	// Initialisation des fonctions dans Lua
	pluginContext->logInfo("Initialisation des fonctions Lua...");

	lua_register(L, "pushEvent", &LuaGlobalMethods::pushEvent);
	lua_register(L, "subscribeEvents", &LuaGlobalMethods::subscribeEvents);
	lua_register(L, "log", &LuaGlobalMethods::log);
	lua_register(L, "logConsoleInfo", &LuaGlobalMethods::logConsoleInfo);

	// Fonctions pour la configuration
	lua_register(L, "saveConfiguration", &PluginConfigurationProxy::saveConfiguration);
	lua_register(L, "getScreenSize", &PluginConfigurationProxy::getScreenSize);
	lua_register(L, "getAvailableAudioDrivers", &PluginConfigurationProxy::getAvailableAudioDrivers);
	lua_register(L, "getAvailableAudioRecordDrivers", &PluginConfigurationProxy::getAvailableAudioRecordDrivers);
	lua_register(L, "getAvailableAudioOutputs", &PluginConfigurationProxy::getAvailableAudioOutputs);
	lua_register(L, "getConfigurationParameter", &PluginConfigurationProxy::getConfigurationParameter);
	lua_register(L, "setConfigurationParameter", &PluginConfigurationProxy::setConfigurationParameter);
	lua_register(L, "isGameModeClient", &PluginConfigurationProxy::isModeClient);
	lua_register(L, "isGameModeLocal", &PluginConfigurationProxy::isModeLocal);
	lua_register(L, "isGameModeServer", &PluginConfigurationProxy::isModeServer);
	lua_register(L, "getConstant", &PluginConfigurationProxy::getConstant);
	lua_register(L, "initAudio", &PluginConfigurationProxy::initAudio);

	// Fonctions d'accès aux données de la partie
	lua_register(L, "getGame", &PluginGameProxy::getGame);

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

	Lunar<PluginGameProxy>::Register(L);
	Lunar<PluginMapProxy>::Register(L);
	Lunar<PluginDataTreeProxy>::Register(L);
	Lunar<PluginDataValeurProxy>::Register(L);
	Lunar<PluginPlayerZoneDetectorProxy>::Register(L);
	Lunar<PluginPlayerProxy>::Register(L);
	Lunar<PluginPlayerZoneEventProxy>::Register(L);

	/* ******************************************************************************
	 * Chargement du fichier Lua du plugin
	 * *****************************************************************************/

	string pluginFile = string(pluginDirectory).append("/").append(pluginName).append(".lua");
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

	// Appel principal pour l'initialisation du script chargé
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
	 * Vérification de la présence d'une fonction "eventManager" dans le plugin
	 * ******************************************************************************/

	lua_getglobal(L, "eventManager");

	if (!lua_isfunction(L, -1)) {
		// la fonction n'existe pas
		pluginContext->logInfo("la fonction 'eventManager' n'existe pas");
	}

	lua_pop(L, 1);


	/* *******************************************************************************
	 * Vérification de la présence de la méthode d'initialisation du plugin ("onLoad")
	 * ******************************************************************************/

	pluginContext->logInfo("Recherche de la fonction onLoad du script...");

	lua_getglobal(L, "onLoad");

	// Vérifie si la fonction d'initialisation du plugin existe bien
	if(!lua_isfunction(L, -1)) {
		// la fonction n'existe pas
		lua_pop(L, 1);

		pluginContext->logError("La fonction 'onLoad' n'existe pas, tout plugin doit définir cette fonction.");
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
		LOGERROR(("Le plugin '%s' est déjà actif", pluginName.c_str()));
		pluginContext->logError("Tentative d'activation du plugin alors qu'il est déjà actif");
		return;
	}

	/* ******************************************************************************
	 * Initialisation des variables
	 * *****************************************************************************/

	const string pluginDirectory = string(PLUGINS_DIRECTORY).append(pluginName).append("/");
	LOGINFO(("Activation of plugin : '%s' in '%s'", pluginName.c_str(), pluginDirectory.c_str()));

	pluginContext = activatePlugin("main", pluginDirectory);

	if(pluginContext) {
		// Référencement du plugin
		lua_State* L = pluginContext->getLuaState();

		_nameGlobalPlugin[pluginName] = pluginContext;
		_luaGlobalContext[L] = pluginContext;

		pluginContext->logInfo("Plugin initialisé");

		// Exécution de la méthode d'init du plugin (onLoad)
		pluginContext->logInfo("Exécution du plugin");

		int status = lua_pcall(L, 0, 0, 0);

		if(status) {
			pluginContext->logLuaError(status);
			pluginContext->logError("Echec d'exécution du plugin.");
		}
		else {
			pluginContext->logInfo("Plugin en cours d'exécution");
		}
	}
}

/**
 * Activate the plugin.
 */
void PluginEngine::activateMapPlugin(CMap* map, const string& pluginName, const string pluginDirectory) {
	PluginContext* pluginContext = getMapPluginContext(pluginName);

	if(pluginContext != NULL) {
		LOGERROR(("Le plugin de Map '%s' est déjà actif", pluginName.c_str()));
		pluginContext->logError("Tentative d'activation du plugin de Map alors qu'il est déjà actif");
		return;
	}

	/* ******************************************************************************
	 * Initialisation des variables
	 * *****************************************************************************/

	LOGINFO(("Activation of Map plugin : '%s' in '%s'", pluginName.c_str(), pluginDirectory.c_str()));

	pluginContext = activatePlugin(pluginName, pluginDirectory);

	if(pluginContext) {
		// Référencement du plugin
		lua_State* L = pluginContext->getLuaState();

		_nameMapPlugin[pluginName] = pluginContext;
		_luaMapContext[L] = pluginContext;

		// Exécution de la méthode d'init du plugin (onLoad)
		pluginContext->logInfo("Exécution du plugin de Map (onLoad)");

		int status = lua_pcall(L, 0, 0, 0);

		if(status) {
			pluginContext->logLuaError(status);
			pluginContext->logError("Echec d'exécution du plugin de Map.");
		}
		else {
			pluginContext->logInfo("Plugin de Map en cours d'exécution");
		}
	}
}

void PluginEngine::deactivateMapPlugins() {
	map<std::string, PluginContext*>::iterator itName;
	map<const lua_State*, PluginContext*>::iterator itContext;

	for(itName = _nameMapPlugin.begin() ; itName != _nameMapPlugin.end() ; itName++) {
		PluginContext* pluginContext = itName->second;

		if(pluginContext != NULL) {
			pluginContext->logInfo("Début de désactivation du plugin de Map");

			lua_close(pluginContext->getLuaState());
			pluginContext->logInfo("Plugin de Map désactivé");

			delete pluginContext;
		}
		else {
			LOGERROR(("Le plugin de Map ne peut pas être désactivé, il n'est pas actif '%s'", itName->first.c_str()));
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
		pluginContext->logInfo("Début de désactivation du plugin");

		lua_close(pluginContext->getLuaState());

		_luaGlobalContext.erase(pluginContext->getLuaState());
		_nameGlobalPlugin.erase(pluginName);

		pluginContext->logInfo("Plugin désactivé");

		delete pluginContext;
	}
	else {
		LOGERROR(("Le plugin ne peut pas être désactivé, il n'est pas actif '%s'", pluginName.c_str()));
	}
}

} /* namespace jkt */
