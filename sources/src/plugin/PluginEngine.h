/*
 * PluginEngine.h
 *
 *  Created on: 1 mai 2012
 *      Author: Erwin
 */

#ifndef PLUGINENGINE_H_
#define PLUGINENGINE_H_

#include <string>
#include <map>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "lunar.h"

#include "plugin/PluginContext.h"

namespace jkt {

class CMap;

class PluginEngine {
	static const std::string PLUGIN_MAIN_FILENAME;

	std::map<std::string, PluginContext*> _nameGlobalPlugin;
	std::map<const lua_State*, PluginContext*> _luaGlobalContext;

	std::map<std::string, PluginContext*> _nameMapPlugin;
	std::map<const lua_State*, PluginContext*> _luaMapContext;

	PluginContext* activatePlugin(const std::string& pluginName, const std::string& pluginDirectory);

public:
	PluginEngine();
	virtual ~PluginEngine();

	PluginContext* getPluginContext(const lua_State* L);

	// Gestion des plugins du jeu
	void activateDefaultGlobalPlugins();
	void activateGlobalPlugin(const std::string& pluginName);
	void deactivateGlobalPlugin(const std::string& pluginName);
	PluginContext* getGlobalPluginContext(const std::string& pluginName);
	PluginContext* getGlobalPluginContext(const lua_State* L);

	// Gestion des plugins de la Map active
	void activateMapPlugin(CMap* map, const std::string& pluginName, const std::string pluginDirectory);
	void deactivateMapPlugins();
	PluginContext* getMapPluginContext(const std::string& pluginName);
	PluginContext* getMapPluginContext(const lua_State* L);

	void dispatchEvent(PluginEventProxy& plugEvent);
	void dispatchEvent(int actionId);
	void sendRefreshEvent();
};

} /* namespace jkt */
#endif /* PLUGINENGINE_H_ */
