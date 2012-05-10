/*
 * PluginContext.cpp
 *
 *  Created on: 9 mai 2012
 *      Author: Erwin
 */

#include "plugin/PluginContext.h"

namespace JktPlugin {

PluginContext::PluginContext(lua_State* luaState) {
	_luaState = luaState;
}

PluginContext::~PluginContext() {
}

lua_State* PluginContext::getLuaState() {
	return _luaState;
}

} /* namespace JktPlugin */
