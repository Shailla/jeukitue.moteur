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
		cerr << endl << __FILE__ << ":" << __LINE__ << " Detail erreur Lua : " << lua_tostring(L, -1);
		lua_pop(L, 1); // remove error message
	}
}


bool LuaUtils::isCheckLuaParametersTypes(lua_State* L, const char* FILE, const char* FUNCTION, int expectedParamNbr, ...) {
	va_list vl;
	va_start(vl, expectedParamNbr);

	// Vérifie si le nombre de paramètres est bon
	int paramNbr = lua_gettop(L);

	if(paramNbr != expectedParamNbr) {
		ostringstream message;
		message << "Mauvais nombre de parametres (attendu=" << expectedParamNbr << ", recus=" << paramNbr << ")";
		Fabrique::getPluginEngine()->getPluginContext(L)->logScriptError(message.str());

		return false;
	}

	// Vérifie les types de chaque paramètre
	for(int i=1 ; i<expectedParamNbr+1 ; i++) {
		const LUA_PARAMETER_TYPES type = (LUA_PARAMETER_TYPES)va_arg(vl, int);

		switch(type) {
		case LUA_PARAM_STRING:
			if(!lua_isstring(L, i)) {
				ostringstream message;
				message << "Mauvais format du parametre " << i << " (attendu=String)";
				Fabrique::getPluginEngine()->getPluginContext(L)->logScriptError(message.str());

				return false;
			}
			break;
		case LUA_PARAM_BOOLEAN:
			if(!lua_isboolean(L, i)) {
				ostringstream message;
				message << "Mauvais format du parametre " << i << " (attendu=Booleen)";
				Fabrique::getPluginEngine()->getPluginContext(L)->logScriptError(message.str());

				return false;
			}
			break;
		case LUA_PARAM_NUMBER:
			if(!lua_isnumber(L, i)) {
				ostringstream message;
				message << "Mauvais format du parametre " << i << " (attendu=Entier)";
				Fabrique::getPluginEngine()->getPluginContext(L)->logScriptError(message.str());

				return false;
			}
			break;
		case LUA_PARAM_USERDATA:
			if(!lua_isuserdata(L, i)) {
				ostringstream message;
				message << "Mauvais format du parametre " << i << " (attendu=Userdata)";
				Fabrique::getPluginEngine()->getPluginContext(L)->logScriptError(message.str());

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

} /* namespace JktPlugin */
