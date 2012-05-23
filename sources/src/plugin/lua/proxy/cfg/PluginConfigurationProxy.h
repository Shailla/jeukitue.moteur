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
public:
	static int getScreenSize(lua_State *L);

	static int isPlayerSkinVisible(lua_State* L);
	static int setPlayerSkinVisibility(lua_State* L);

	static int isPlayerOutlineVisible(lua_State* L);
	static int setPlayerOutlineVisibility(lua_State* L);

	static int isCubicMeterVisible(lua_State* L);
	static int setCubicMeterVisibility(lua_State* L);

	static int isAxesMeterVisible(lua_State* L);
	static int setAxesMeterVisibility(lua_State* L);
};

} /* namespace JktPlugin */
#endif /* PLUGINCONFIGURATIONPROXY_H_ */
