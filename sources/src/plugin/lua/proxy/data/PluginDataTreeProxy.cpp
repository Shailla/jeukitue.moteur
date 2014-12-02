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

#include "util/types/IntData.h"
#include "util/types/FloatData.h"
#include "util/types/StringData.h"
#include "data/DataTree.h"
#include "main/Fabrique.h"
#include "plugin/lua/LuaUtils.h"
#include "plugin/PluginEngine.h"
#include "main/Game.h"

#include "plugin/lua/proxy/data/PluginDataTreeProxy.h"

extern CGame Game;			// Contient toutes les données vivantes du jeu

namespace JktPlugin {

const char PluginDataTreeProxy::className[] = "DataTree";

PluginDataTreeProxy* PluginDataTreeProxy::_this = 0;

Lunar<PluginDataTreeProxy>::RegType PluginDataTreeProxy::methods[] = {
		{"createValeur", &PluginDataTreeProxy::createValeur},
		{"createBranche", &PluginDataTreeProxy::createBranche},
		{0}
};

PluginDataTreeProxy::PluginDataTreeProxy() {
}

PluginDataTreeProxy::PluginDataTreeProxy(lua_State* L) {

}

PluginDataTreeProxy* PluginDataTreeProxy::getInstance() {
	if(!_this) {
		_this = new PluginDataTreeProxy();
	}

	return _this;
}

/**
 * .
 *    - Return 1 :
 */
int PluginDataTreeProxy::getMapDataTree(lua_State *L) {
	LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 0);

	return Lunar<PluginDataTreeProxy>::push(L, getInstance());
}

/**
 * Create a new valeur in the data tree in a branche.
 * 	- Param 1 <brancheId> : branche id in which the valeur will be created
 * 	- Param 2 <valeurType> : type of the value ("int", "float", "string")
 * 	- Param 3 <valeurName> : name of the valeur
 *  - Return 1 <valeurId> : id of the new valeur
 */
int PluginDataTreeProxy::createValeur(lua_State *L) {
//	LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 3, LUA_PARAM_ARRAY_INT, LUA_PARAM_STRING, LUA_PARAM_STRING);

	vector<int> brancheId;

	int index = 1;

	lua_pushnil(L);

	while(lua_next(L, index)) {
	    if(lua_isnumber(L, -1)) {
	        int var = (int)lua_tonumber(L, -1);
	        brancheId.push_back(var);
	    }

	    lua_pop(L, 1);
	}

	index++;

	const string valeurType = lua_tostring(L, index++);

	const string valeurName = lua_tostring(L, index++);

	DataTree* dataTree = Game.getDataTree();
	const JktUtils::Data* data;

	if(valeurType == "int") {
		data = new JktUtils::IntData(0);
	}
	else if(valeurType == "float") {
		data = new JktUtils::FloatData(0);
	}
	else if(valeurType == "string") {
		data = new JktUtils::StringData(0);
	}
	else {
		ostringstream message;
		message << "Type de valeur " << valeurType << " inconnu";
		Fabrique::getPluginEngine()->getGlobalPluginContext(L)->logScriptError(message.str());
	}

	Valeur* valeur = dataTree->createValeur(brancheId, valeurName.c_str(), data);

	vector<int> valeurBrancheId = valeur->getBrancheId();
	int valeurId = valeur->getValeurId();

	lua_createtable(L, valeurBrancheId.size() + 1, 0);

	for(int i=0; i<valeurBrancheId.size(); i++) {
		lua_pushinteger(L, valeurBrancheId.at(i));
		lua_rawseti (L, -2, i);
	}

	lua_pushinteger(L, valeurId);
	lua_rawseti (L, -2, valeurBrancheId.size());

	return 1;
}

/**
 * Create a new branche in the data tree in a parent branche.
 * 	- Param 1 <parentBrancheId> : branche id in which the new branche will be created
 * 	- Param 3 <brancheName> : name of the branche
 *  - Return 1 <brancheId> : id of the new branche
 */
int PluginDataTreeProxy::createBranche(lua_State *L) {
//	LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 3, LUA_PARAM_ARRAY_INT, LUA_PARAM_NUMBER, LUA_PARAM_STRING);

	int index = 1;

	vector<int> parentBrancheId;

	lua_pushnil(L);

	while(lua_next(L, index)) {
	    if(lua_isnumber(L, -1)) {
	        int var = (int)lua_tonumber(L, -1);
	        parentBrancheId.push_back(var);
	    }

	    lua_pop(L, 1);
	}

	index++;

	const string brancheName = lua_tostring(L, index++);

	DataTree* dataTree = Game.getDataTree();

	Branche* branche = dataTree->createBranche(parentBrancheId, brancheName.c_str());

	vector<int> brancheId = branche->getBrancheFullId();

	lua_createtable(L, brancheId.size() + 1, 0);

	for(int i=0; i<brancheId.size(); i++) {
		lua_pushinteger(L, brancheId.at(i));
		lua_rawseti (L, -2, i);
	}

	return 1;
}

} /* namespace JktPlugin */
