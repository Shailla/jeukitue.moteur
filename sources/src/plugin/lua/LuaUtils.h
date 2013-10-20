/*
 * LuaUtils.h
 *
 *  Created on: 11 mai 2012
 *      Author: Erwin
 */

#ifndef LUAUTILS_H_
#define LUAUTILS_H_

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "lunaFive.h"

namespace JktPlugin {

enum LUA_PARAMETER_TYPES {
	LUA_PARAM_BOOLEAN,	// Paramètre de type booléen
	LUA_PARAM_NUMBER,	// Paramètre de type nombre entier
	LUA_PARAM_STRING,	// Paramètre de type chaîne de caractères
	LUA_PARAM_USERDATA,	//
	LUA_PARAM_ANY		// Paramètre de type indifférent, n'importe quel type de paramètre
};

class LuaUtils {
	LuaUtils();	// Singleton
public:
	virtual ~LuaUtils();

	void static report_lua_errors(lua_State *L, int status);

	bool static isCheckLuaParametersTypes(lua_State* L, const char* FILE, const char* FUNCTION, int expectedParamNbr, ...);
};

} /* namespace JktPlugin */
#endif /* LUAUTILS_H_ */
