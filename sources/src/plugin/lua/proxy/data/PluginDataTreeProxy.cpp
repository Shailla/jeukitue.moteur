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

	lua_pushnil(L);

	while(lua_next(L, -2)) {
	    if(lua_isnumber(L, -2)) {
	        brancheId.push_back((int)lua_tonumber(L, -2));
	    }
	    lua_pop(L, 1);
	}
	lua_pop(L, 1);

	const string valeurType = lua_tostring(L, 1);
	lua_pop(L, 1);

	const string valeurName = lua_tostring(L, 1);
	lua_pop(L, 1);

	DataTree* dataTree = Game.getDataTree();
	const JktUtils::Data* valeur;

	if(valeurType == "int") {
		valeur = new JktUtils::IntData(0);
	}
	else if(valeurType == "float") {
		valeur = new JktUtils::FloatData(0);
	}
	else if(valeurType == "string") {
		valeur = new JktUtils::StringData(0);
	}

	dataTree->createValeur(brancheId, valeurName.c_str(), valeur);

	return 0;
}

/**
 * Create a new branche in the data tree in a parent branche.
 * 	- Param 1 <parentBrancheId> : branche id in which the new branche will be created
 * 	- Param 3 <brancheName> : name of the branche
 *  - Return 1 <brancheId> : id of the new branche
 */
int PluginDataTreeProxy::createBranche(lua_State *L) {
	LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 3, LUA_PARAM_ARRAY_INT, LUA_PARAM_NUMBER, LUA_PARAM_STRING);

	int index = 1;

	vector<int> brancheId;

	lua_pushnil(L);

	while(lua_next(L, (index++)-2)) {  // <== here is your mistake
	    if(lua_isnumber(L, (index++)-1)) {
	        int i = (int)lua_tonumber(L, (index++)-1);
	        //use number
	    }
	}

	const string brancheName = lua_tostring(L, index++);

	DataTree* dataTree = Game.getDataTree();

	dataTree->createBranche(brancheId, brancheName.c_str());

	return 2;
}

} /* namespace JktPlugin */
