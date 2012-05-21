/*
 * LuaUtils.cpp
 *
 *  Created on: 11 mai 2012
 *      Author: Erwin
 */

#include <iostream>

using namespace std;

#include "plugin/lua/LuaUtils.h"

namespace JktPlugin {

LuaUtils::LuaUtils() {
}

LuaUtils::~LuaUtils() {
}

/**
 * Report Lua execution errors.
 */
void LuaUtils::report_lua_errors(lua_State *L, int status) {
	if (status != 0) {
		cerr << endl << "Detail erreur Lua : " << lua_tostring(L, -1);
		lua_pop(L, 1); // remove error message
	}
}


bool LuaUtils::isCheckLuaParametersTypes(lua_State* L, const char* FILE, const char* FUNCTION, int typesNbr, ...) {
	va_list vl;
	va_start(vl, typesNbr);

	// Vérifie si le nombre de paramètres est bon
	if(lua_gettop(L) != typesNbr) {
		cerr << endl << "Erreur Lua (" << FILE << ":" << FUNCTION << ") : Mauvais nombre de paramètres";
	}

	// Vérifie les types de chaque paramètre
	for(int i=0 ; i<typesNbr ; i++) {
		const LUA_PARAMETER_TYPES type = (LUA_PARAMETER_TYPES)va_arg(vl, int);

		switch(type) {
		case LUA_PARAM_STRING:
			if(!lua_isstring(L, i+1)) {
				return false;
			}
			break;
		case LUA_PARAM_BOOLEAN:
			if(!lua_isboolean(L, i+1)) {
				return false;
			}
			break;
		case LUA_PARAM_NUMBER:
			if(!lua_isnumber(L, i+1)) {
				return false;
			}
			break;
		default:
			cerr << endl << "Erreur Lua : Type de parametre Lua non-reconnu";
			return false;
		}
	}

	va_end(vl);

	return true;
}

void LuaUtils::checkLuaParametersTypes(lua_State* L, const char* FILE, const char* FUNCTION, int typesNbr, ...) {
	va_list vl;
	va_start( vl, typesNbr );

	// Vérifie si le nombre de paramètres est bon
	if(lua_gettop(L) != typesNbr) {
		cerr << endl << "Erreur Lua (" << FILE << ":" << FUNCTION << ") : Mauvais nombre de paramètres";
	}

	// Vérifie les types de chaque paramètre
	for(int i=0 ; i<typesNbr ; i++) {
		const LUA_PARAMETER_TYPES type = (LUA_PARAMETER_TYPES)va_arg(vl, int);

		switch(type) {
		case LUA_PARAM_STRING:
			if(!lua_isstring(L, i+1)) {
				cerr << endl << "Erreur Lua : Mauvais type de parametre";
			}
			break;
		case LUA_PARAM_NUMBER:
			if(!lua_isnumber(L, i+1)) {
				cerr << endl << "Erreur Lua : Mauvais type de parametre";
			}
			break;
		default:
			cerr << endl << "Erreur Lua : Type de parametre Lua non-reconnu";
			break;
		}
	}

	va_end(vl);
}

} /* namespace JktPlugin */
