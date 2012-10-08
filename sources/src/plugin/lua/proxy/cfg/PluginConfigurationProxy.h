/*
 * PluginConfigurationProxy.h
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#ifndef PLUGINCONFIGURATIONPROXY_H_
#define PLUGINCONFIGURATIONPROXY_H_

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "lunar.h"

namespace JktPlugin {

class PluginConfigurationProxy {
	PluginConfigurationProxy() {}		// Singleton

	static const char* CFG_PARAM_SKIN_VISIBILITY;			// Paramètre de type booléen
	static const char* CFG_PARAM_PLAYER_OUTLINE_VISIBILITY;	// Paramètre de type booléen
	static const char* CFG_PARAM_CUBIC_METER_VISIBILITY;	// Paramètre de type booléen
	static const char* CFG_PARAM_AXES_METER_VISIBILITY;		// Paramètre de type booléen

public:
	static int getScreenSize(lua_State *L);

	static int getConfigurationParameter(lua_State* L);
	static int setConfigurationParameter(lua_State* L);
};

} /* namespace JktPlugin */
#endif /* PLUGINCONFIGURATIONPROXY_H_ */
