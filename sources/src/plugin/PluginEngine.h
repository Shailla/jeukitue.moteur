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

namespace JktPlugin {

class PluginEngine {
	static const std::string PLUGIN_MAIN_FILENAME;

	std::map<std::string, PluginContext*> _nameGlobalPlugin;
	std::map<const lua_State*, PluginContext*> _luaGlobalContext;

	std::map<std::string, PluginContext*> _nameMapPlugin;
	std::map<const lua_State*, PluginContext*> _luaMapContext;

	PluginContext* activatePlugin(const string& pluginName, const string& pluginDirectory);

public:
	PluginEngine();
	virtual ~PluginEngine();

	// Gestion des plugins du jeu
	void activateDefaultGlobalPlugins();
	void activateGlobalPlugin(const std::string& pluginName);
	void deactivateGlobalPlugin(const std::string& pluginName);
	PluginContext* getGlobalPluginContext(const std::string& pluginName);
	PluginContext* getGlobalPluginContext(const lua_State* L);

	// Gestion des plugins de la Map active
	void activateMapPlugin(const string& pluginName, const string pluginDirectory);
	void deactivateMapPlugins();
	PluginContext* getMapPluginContext(const std::string& pluginName);
	PluginContext* getMapPluginContext(const lua_State* L);

	void dispatchEvent(const PluginActionEvent& event);
};

} /* namespace JktPlugin */
#endif /* PLUGINENGINE_H_ */
