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
	std::map<string, PluginContext*> _pluginMap;

public:
	PluginEngine();
	virtual ~PluginEngine();

	void activatePlugin(const string& pluginName);
	void deactivatePlugin(const string& pluginName);
	void executePlugin(const string& pluginName);
};

} /* namespace JktPlugin */
#endif /* PLUGINENGINE_H_ */
