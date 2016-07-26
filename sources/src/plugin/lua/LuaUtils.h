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
	LUA_PARAM_BOOLEAN,	// Paramètre de type booléen
	LUA_PARAM_NUMBER,	// Paramètre de type nombre entier
	LUA_PARAM_STRING,	// Paramètre de type chaîne de caractères
	LUA_PARAM_USERDATA,	//
	LUA_PARAM_ARRAY_INT,	// Paramètre de type tableau d'entiers
	LUA_PARAM_ANY		// Paramètre de type indifférent, n'importe quel type de paramètre
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
