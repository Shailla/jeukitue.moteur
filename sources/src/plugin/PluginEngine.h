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

	std::map<std::string, PluginContext*> _mapNamePlugin;
	std::map<const lua_State*, PluginContext*> _mapLuaContext;

public:
	PluginEngine();
	virtual ~PluginEngine();

	void activateDefaultPlugins();
	void activatePlugin(const std::string& pluginName);
	void deactivatePlugin(const std::string& pluginName);

	PluginContext* getPluginContext(const std::string& pluginName);
	PluginContext* getPluginContext(const lua_State* L);

	void dispatchEvent(const PluginActionEvent& event);
};

} /* namespace JktPlugin */
#endif /* PLUGINENGINE_H_ */
