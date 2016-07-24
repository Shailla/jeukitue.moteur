/*
 * LuaUtils.cpp
 *
 *  Created on: 11 mai 2012
 *      Author: Erwin
 */

#include <iostream>
#include <sstream>

using namespace std;

#include "main/Fabrique.h"
#include "plugin/PluginEngine.h"

#include "plugin/lua/LuaUtils.h"

namespace jkt {

LuaUtils::LuaUtils() {
}

LuaUtils::~LuaUtils() {
}

/**
 * Report Lua execution errors.
 */
void LuaUtils::report_lua_errors(lua_State *L, int status) {
	if (status != 0) {
		cerr << endl << __FILE__ << ":" << __LINE__ << " Detail erreur Lua : " << lua_tostring(L, -1);
		lua_pop(L, 1); // remove error message
	}
}

void LuaUtils::readStringArray(lua_State *L, vector<string>& array, int& index) {
	lua_pushnil(L);

	while(lua_next(L, index)) {
	    if(lua_isstring(L, -1)) {
	        const char* var = lua_tostring(L, -1);
	        array.push_back(var);
	    }

	    lua_pop(L, 1);
	}

	index++;
}

void LuaUtils::readIntArray(lua_State *L, vector<int>& array, int& index) {
	lua_pushnil(L);

	while(lua_next(L, index)) {
	    if(lua_isnumber(L, -1)) {
	        int var = (int)lua_tonumber(L, -1);
	        array.push_back(var);
	    }

	    lua_pop(L, 1);
	}

	index++;
}

void LuaUtils::pushIntArray(lua_State *L, const vector<int> array) {
	lua_createtable(L, array.size() + 1, 0);

	vector<int>::const_iterator iter;

	int index = 1;

	for(iter = array.begin() ; iter != array.end() ; iter++) {
		int var = *iter;
		lua_pushinteger(L, var);
		lua_rawseti (L, -2, index++);
	}
}

void LuaUtils::pushIntArray(lua_State *L, const vector<int> array, int oneMoreValue) {
	lua_createtable(L, array.size() + 2, 0);

	vector<int>::const_iterator iter;

	int index = 1;

	for(iter = array.begin() ; iter != array.end() ; iter++) {
		int var = *iter;
		lua_pushinteger(L, var);
		lua_rawseti (L, -2, index++);
	}

	lua_pushinteger(L, oneMoreValue);
	lua_rawseti (L, -2, index++);
}

bool LuaUtils::isCheckLuaParametersTypes(lua_State* L, const char* FILE, const char* FUNCTION, int expectedParamNbr, ...) {
	va_list vl;
	va_start(vl, expectedParamNbr);

	// Vérifie si le nombre de paramètres est bon
	int paramNbr = lua_gettop(L);

	if(paramNbr != expectedParamNbr) {
		ostringstream message;
		message << "Mauvais nombre de parametres (attendu=" << expectedParamNbr << ", recus=" << paramNbr << ")";
		Fabrique::getPluginEngine()->getGlobalPluginContext(L)->logScriptError(message.str());

		return false;
	}

	int index = 1;

	// Vérifie les types de chaque paramètre
	for(int id=0 ; id<expectedParamNbr ; id++) {
		const LUA_PARAMETER_TYPES type = (LUA_PARAMETER_TYPES)va_arg(vl, int);

		switch(type) {
		case LUA_PARAM_STRING:
			if(!lua_isstring(L, index++)) {
				ostringstream message;
				message << "Mauvais format du parametre " << index << " (attendu=String, obtenu=" << lua_typename(L, index-1) << ")";
				Fabrique::getPluginEngine()->getGlobalPluginContext(L)->logScriptError(message.str());

				return false;
			}
			break;
		case LUA_PARAM_BOOLEAN:
			if(!lua_isboolean(L, index++)) {
				ostringstream message;
				message << "Mauvais format du parametre " << index << " (attendu=Booleen, obtenu=" << lua_typename(L, index-1) << ")";
				Fabrique::getPluginEngine()->getGlobalPluginContext(L)->logScriptError(message.str());

				return false;
			}
			break;
		case LUA_PARAM_ARRAY_INT:
			if(!lua_istable(L, index)) {
				ostringstream message;
				message << "Mauvais format du parametre " << index << " (attendu=Table, obtenu=" << lua_typename(L, index-1) << ")";
				Fabrique::getPluginEngine()->getGlobalPluginContext(L)->logScriptError(message.str());

				return false;
			}
			break;
		case LUA_PARAM_NUMBER:
			if(!lua_isnumber(L, index++)) {
				ostringstream message;
				message << "Mauvais format du parametre " << index << " (attendu=Entier, obtenu=" << lua_typename(L, index-1) << ")";
				Fabrique::getPluginEngine()->getGlobalPluginContext(L)->logScriptError(message.str());

				return false;
			}
			break;
		case LUA_PARAM_USERDATA:
			if(!lua_isuserdata(L, index++)) {
				ostringstream message;
				message << "Mauvais format du parametre " << index << " (attendu=Userdata, obtenu=" << lua_typename(L, index-1) << ")";
				Fabrique::getPluginEngine()->getGlobalPluginContext(L)->logScriptError(message.str());

				return false;
			}
			break;
		case LUA_PARAM_ANY:
			// Nothing to do
			break;
		default:
			cerr << endl << __FILE__ << ":" << __LINE__ << " Erreur Lua : Type de parametre Lua non-reconnu";
			return false;
		}
	}

	va_end(vl);

	return true;
}

} /* namespace jkt */
