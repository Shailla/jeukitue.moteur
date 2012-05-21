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

#include "lunar.h"

namespace JktPlugin {

enum LUA_PARAMETER_TYPES {
	LUA_PARAM_BOOLEAN,
	LUA_PARAM_NUMBER,
	LUA_PARAM_STRING
};

class LuaUtils {
	LuaUtils();	// Singleton
public:
	virtual ~LuaUtils();

	void static report_lua_errors(lua_State *L, int status);

	bool static isCheckLuaParametersTypes(lua_State* L, const char* FILE, const char* FUNCTION, int typesNbr, ...);
	void static checkLuaParametersTypes(lua_State* L, const char* FILE, const char* FUNCTION, int typesNbr, ...);
};

} /* namespace JktPlugin */
#endif /* LUAUTILS_H_ */
