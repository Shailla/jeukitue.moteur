/*
 * PluginContext.cpp
 *
 *  Created on: 9 mai 2012
 *      Author: Erwin
 */

#include <agar/core.h>

#include "util/Trace.h"
#include "plugin/lua/proxy/PluginEventProxy.h"

#include "plugin/PluginContext.h"

using namespace std;

namespace jkt {

const char* PluginContext::LOG_USER_PREFIX = 		"USER   : ";
const char* PluginContext::LOG_INFO_PREFIX = 		"INFO   : ";
const char* PluginContext::LOG_ERROR_PREFIX = 		"ERREUR : ";
const char* PluginContext::LOG_ERROR_LUA_PREFIX = 	"ERREUR LUA : ";

PluginContext::PluginContext(lua_State* luaState, const string& pluginName, const string& pluginsDirectory) : _pluginName(pluginName) {
	_luaState = luaState;

	string pluginLogFile = string(pluginsDirectory).append(pluginName).append(".log");

	_logFile.exceptions(std::ofstream::failbit|std::ofstream::badbit);

	try {
		_logFile.open(pluginLogFile.c_str());
	}
	catch(std::ios_base::failure& exception) {
		LOGERROR(("Cannot open log file '%s'", pluginLogFile.c_str()));
	}

	_subscribedRefreshEvents = false;

	_dispatchEventMutex = SDL_CreateMutex();
}

PluginContext::~PluginContext() {
	// Fermeture propre du fichier de logs du plugin
	_logFile.flush();
	_logFile.close();
}

lua_State* PluginContext::getLuaState() {
	return _luaState;
}

/**
 * Emet une trace demand� par le script Lua dans le fichier de log du plugin.
 */
void PluginContext::logUser(const string& trace) {
	_logFile << endl << LOG_USER_PREFIX << trace.c_str() << flush;
}

/**
 * Emet une trace d'information dans le fichier de log du plugin.
 */
void PluginContext::logInfo(const string& trace) {
	_logFile << endl << LOG_INFO_PREFIX << trace.c_str() << flush;
}

/**
 * Emet une trace d'erreur dans le fichier de log du plugin.
 */
void PluginContext::logScriptError(const string& trace) {
	// R�cup�re le num�ro de la ligne en cours d'ex�cution dans le code LUA du plugin
	lua_Debug ar;
	lua_getstack(_luaState, 1, &ar);
	lua_getinfo(_luaState, "l", &ar);

	_logFile << endl << LOG_ERROR_PREFIX << "(ligne " << ar.currentline << ") " << trace.c_str() << flush;
}

/**
 * Emet une trace d'erreur dans le fichier de log du plugin.
 */
void PluginContext::logError(const string& trace) {
	_logFile << endl << LOG_ERROR_PREFIX << trace << flush;
}

/**
 * Si une erreur Lua s'est produite alors �met une trace dans le fichier de log du plugin.
 */
void PluginContext::logLuaError(const int status) {
	if (status != 0) {
		_logFile << endl << LOG_ERROR_LUA_PREFIX << lua_tostring(_luaState, -1) << flush;
		lua_pop(_luaState, 1);
	}
}

void PluginContext::dispatchEvent(PluginWidgetEvent* event) {
	SDL_LockMutex(_dispatchEventMutex);

	PluginEventProxy* eventProxy = new PluginEventProxy(event);
	delete event;

	lua_getglobal(_luaState, "eventManager");

	// on v�rifie si la fonction existe bien
	if (lua_isfunction(_luaState, -1)) {
		Lunar<PluginEventProxy>::push(_luaState, eventProxy, true);

		int status = lua_pcall(_luaState, 1, 0, 0);
		logLuaError(status);
	}

	SDL_UnlockMutex(_dispatchEventMutex);
}

bool PluginContext::isSubscribedRefreshEvents() const {
	return _subscribedRefreshEvents;
}

void PluginContext::dispatchEvent(const PluginActionEvent& event) {
	SDL_LockMutex(_dispatchEventMutex);

	PluginEventProxy* eventProxy = new PluginEventProxy(event);

	lua_getglobal(_luaState, "eventManager");

	// on v�rifie si la fonction existe bien
	if (lua_isfunction(_luaState, -1)) {
		Lunar<PluginEventProxy>::push(_luaState, eventProxy, true);

		int status = lua_pcall(_luaState, 1, 0, 0);
		logLuaError(status);
	}

	SDL_UnlockMutex(_dispatchEventMutex);
}

void PluginContext::subscribeRefreshEvents(const bool state) {
	SDL_LockMutex(_dispatchEventMutex);

	_subscribedRefreshEvents = state;

	SDL_UnlockMutex(_dispatchEventMutex);
}

} /* namespace jkt */
