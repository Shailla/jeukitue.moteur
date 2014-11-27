/*
 * PluginDataTreeProxy.cpp
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#include <iostream>
#include <sstream>

using namespace std;

#include "fmod.h"

#include "main/Fabrique.h"
#include "plugin/lua/LuaUtils.h"
#include "plugin/PluginEngine.h"

#include "plugin/lua/proxy/data/PluginDataTreeProxy.h"

namespace JktPlugin {

/**
 * .
 *    - Return 1 :
 */
int PluginDataTreeProxy::getMapDataTree(lua_State *L) {
	LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 0);
	const SDL_VideoInfo* videoInfo = SDL_GetVideoInfo();

	lua_pushnumber(L, videoInfo->current_w);
	lua_pushnumber(L, videoInfo->current_h);

	return 2;
}

} /* namespace JktPlugin */
