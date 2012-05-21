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
	static std::map<std::string, PluginContext*> _mapNamePlugin;
	static std::map<const lua_State*, PluginContext*> _mapLuaContext;

public:
	PluginEngine();
	virtual ~PluginEngine();

	void activatePlugin(std::string& pluginName);
	void deactivatePlugin(std::string& pluginName);

	static PluginContext* getPluginContext(std::string& pluginName);
	static PluginContext* getPluginContext(const lua_State* L);
};

} /* namespace JktPlugin */
#endif /* PLUGINENGINE_H_ */
