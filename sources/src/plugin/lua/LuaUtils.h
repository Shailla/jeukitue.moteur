/*
 * LuaUtils.h
 *
 *  Created on: 11 mai 2012
 *      Author: Erwin
 */

#ifndef LUAUTILS_H_
#define LUAUTILS_H_

#include <string>
#include <vector>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "lunar.h"

using namespace std;

namespace jkt {

enum LUA_PARAMETER_TYPES {
	LUA_PARAM_BOOLEAN,	// Param�tre de type bool�en
	LUA_PARAM_NUMBER,	// Param�tre de type nombre entier
	LUA_PARAM_STRING,	// Param�tre de type cha�ne de caract�res
	LUA_PARAM_USERDATA,	//
	LUA_PARAM_ARRAY_INT,	// Param�tre de type tableau d'entiers
	LUA_PARAM_ANY		// Param�tre de type indiff�rent, n'importe quel type de param�tre
};

class LuaUtils {
	LuaUtils();	// Singleton
public:
	virtual ~LuaUtils();

	static void report_lua_errors(lua_State *L, int status);

	static bool isCheckLuaParametersTypes(lua_State* L, const char* FILE, const char* FUNCTION, int expectedParamNbr, ...);

	static void readStringArray(lua_State *L, std::vector<std::string>& array, int& index);
	static void readIntArray(lua_State *L, std::vector<int>& array, int& index);
	static void pushIntArray(lua_State *L, const std::vector<int> array, int oneMoreValue);
	static void pushIntArray(lua_State *L, const std::vector<int> array);
};

} /* namespace jkt */
#endif /* LUAUTILS_H_ */
