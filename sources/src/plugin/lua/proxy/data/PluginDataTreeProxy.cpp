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
 * .
 *    - Return 1 :
 */
int PluginDataTreeProxy::createValeur(lua_State *L) {
	LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 2, LUA_PARAM_STRING, LUA_PARAM_STRING);

	const string valeurType = lua_tostring(L, 1);
	const string valeurName = lua_tostring(L, 2);

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

	dataTree->createValeur(0, valeurName.c_str(), valeur);

	return 2;
}

int PluginDataTreeProxy::createBranche(lua_State *L) {
	LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 3, LUA_PARAM_NUMBER, LUA_PARAM_STRING);

	int index = 1;

	vector<int> brancheId;

	int nbr = lua_tonumber(L, index++);

	for(int i=0 ; i<nbr ; i++) {
		brancheId.push_back(lua_tonumber(L, index++));
	}

	const string brancheName = lua_tostring(L, index++);

	DataTree* dataTree = Game.getDataTree();

	dataTree->createBranche(brancheId, brancheName.c_str());

	return 2;
}

} /* namespace JktPlugin */