/*
 * PluginEngine.cpp
 *
 *  Created on: 1 mai 2012
 *      Author: Erwin
 */

#include <iostream>

using namespace std;

#include "plugin/api/gui/LuaWindowProxy.h"

#include "plugin/PluginEngine.h"

extern const char* PLUGINS_DIRECTORY;

namespace JktPlugin {

PluginEngine::PluginEngine() {
}

PluginEngine::~PluginEngine() {
}

void report_lua_errors(lua_State *L, int status) {
	if (status != 0) {
		cerr << "-- " << lua_tostring(L, -1) << endl;
		lua_pop(L, 1); // remove error message
	}
}

void PluginEngine::activatePlugin(const string& pluginName) {
	PluginContext* pluginContext = _pluginMap[pluginName];

	if(pluginContext == NULL) {
		lua_State* luaState = lua_open();

		luaopen_io(luaState); 		// provides io.*
		luaopen_base(luaState);
		luaopen_table(luaState);
		luaopen_string(luaState);
		luaopen_math(luaState);
		luaopen_loadlib(luaState);

		// Ouverture du fichier Lua
		string pluginPath = string(PLUGINS_DIRECTORY).append(pluginName).append("/").append(pluginName).append(".lua");
		cout << endl << "Activation of plugin : '" << pluginPath << "'";
		int result = luaL_loadfile(luaState, pluginPath.c_str());

		// Déclaration des fonctions Lua
		Lunar<LuaWindowProxy>::Register(luaState);

		if(result == 0) {
			pluginContext = new PluginContext(luaState);
			_pluginMap[pluginName] = pluginContext;
		}
		else {
			cerr << endl << "Echec d'ouverture du fichier du plugin '" << pluginName << "'";
		}
	}
	else {
		cerr << endl << "Le plugin est déjà actif '" << pluginName << "'";
	}
}

void PluginEngine::executePlugin(const string& pluginName) {
	PluginContext* pluginContext = _pluginMap[pluginName];

	if(pluginContext != NULL) {
		lua_State* state = pluginContext->getLuaState();
		int result = lua_pcall(state, 0, LUA_MULTRET, 0);

		report_lua_errors(state, result);
	}
}

void PluginEngine::deactivatePlugin(const string& pluginName) {
	PluginContext* pluginContext = _pluginMap[pluginName];

	if(pluginContext != NULL) {
		lua_State* luaState = pluginContext->getLuaState();
		lua_close(luaState);
		_pluginMap.erase(pluginName);
		delete pluginContext;
	}
}

} /* namespace JktPlugin */
