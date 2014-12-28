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

#include "data/DataTree.h"
#include "main/Fabrique.h"
#include "plugin/lua/LuaUtils.h"
#include "plugin/PluginEngine.h"
#include "main/Game.h"
#include "plugin/lua/proxy/data/PluginDataValeurProxy.h"

#include "plugin/lua/proxy/data/PluginDataTreeProxy.h"

extern CGame Game;			// Contient toutes les données vivantes du jeu

namespace JktPlugin {

const char PluginDataTreeProxy::className[] = "DataTree";

Lunar<PluginDataTreeProxy>::RegType PluginDataTreeProxy::methods[] = {
		{"createValeur", &PluginDataTreeProxy::createValeur},
		{"getBranche", &PluginDataTreeProxy::getBranche},
		{"createBranche", &PluginDataTreeProxy::createBranche},
		{0}
};

PluginDataTreeProxy::PluginDataTreeProxy(DataTree* dataTree) {
	_dataTree = dataTree;
}

PluginDataTreeProxy::PluginDataTreeProxy(lua_State* L) {
	_dataTree = 0;
}

/**
 * .
 *    - Return 1 :
 */
int PluginDataTreeProxy::getDataTree(lua_State *L) {
	LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 0);

	DataTree* dataTree = Game.getDataTree();
	return Lunar<PluginDataTreeProxy>::push(L, new PluginDataTreeProxy(dataTree));
}

/**
 */
int PluginDataTreeProxy::getBranche(lua_State *L) {
//	LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 3, LUA_PARAM_ARRAY_INT, LUA_PARAM_STRING, LUA_PARAM_STRING);

	vector<string> branchePath;

	int index = 1;

	LuaUtils::readStringArray(L, branchePath, index);

	Branche* branche = _dataTree->getBranche(branchePath);

	LuaUtils::pushIntArray(L, branche->getBrancheFullId());

	return 1;
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

	LuaUtils::readIntArray(L, brancheId, index);

	const string valeurType = lua_tostring(L, index++);

	const string valeurName = lua_tostring(L, index++);

	JktUtils::AnyData data;

	if(valeurType == "int") {
		data.update((int)0);
	}
	else if(valeurType == "float") {
		data.update((float)0);
	}
	else if(valeurType == "string") {
		data.update(string(""));
	}
	else {
		ostringstream message;
		message << "Type de valeur " << valeurType << " inconnu";
		Fabrique::getPluginEngine()->getGlobalPluginContext(L)->logScriptError(message.str());
	}

	Valeur* valeur = _dataTree->createValeur(brancheId, valeurName.c_str(), data);
	PluginDataValeurProxy* valeurProxy = new PluginDataValeurProxy(valeur);

	Lunar<PluginDataValeurProxy>::push(L, valeurProxy);

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

	// Get input values
	int index = 1;

	vector<int> parentBrancheId;
	LuaUtils::readIntArray(L, parentBrancheId, index);

	const string brancheName = lua_tostring(L, index++);

	// Do staff
	DataTree* dataTree = Game.getDataTree();
	Branche* branche = dataTree->createBranche(parentBrancheId, brancheName.c_str());

	// Push return values
	vector<int> brancheId = branche->getBrancheFullId();
	LuaUtils::pushIntArray(L, brancheId);

	return 1;
}

} /* namespace JktPlugin */
