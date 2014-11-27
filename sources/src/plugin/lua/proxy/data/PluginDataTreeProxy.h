/*
 * PluginDataTreeProxy.h
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#ifndef PLUGINDATATREEPROXY_H_
#define PLUGINDATATREEPROXY_H_

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

namespace JktPlugin {

class PluginDataTreeProxy {
	PluginDataTreeProxy() {}		// Singleton

public:
	static int getMapDataTree(lua_State *L);
};

} /* namespace JktPlugin */
#endif /* PLUGINDATATREEPROXY_H_ */
