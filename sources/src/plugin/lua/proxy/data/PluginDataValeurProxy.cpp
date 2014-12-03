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
#include "data/ValeurFloat.h"
#include "plugin/lua/LuaUtils.h"
#include "plugin/PluginEngine.h"
#include "main/Game.h"

#include "plugin/lua/proxy/data/PluginDataValeurProxy.h"

namespace JktPlugin {

const char PluginDataValeurProxy::className[] = "DataValeur";

Lunar<PluginDataValeurProxy>::RegType PluginDataValeurProxy::methods[] = {
		{"getValue", &PluginDataValeurProxy::getValue},
		{"setValue", &PluginDataValeurProxy::setValue},
		{"getBrancheId", &PluginDataValeurProxy::getBrancheId},
		{"getValeurFullId", &PluginDataValeurProxy::getValeurFullId},
		{0}
};

PluginDataValeurProxy::PluginDataValeurProxy(Valeur* valeur) {
	_valeur = valeur;
}

PluginDataValeurProxy::PluginDataValeurProxy(lua_State* L) {
}

/**
 * .
 *    - Return 1 :
 */
int PluginDataValeurProxy::getValue(lua_State *L) {
	if(dynamic_cast<ValeurFloat*>(_valeur)) {
		 JktUtils::FloatData* data = (JktUtils::FloatData*)_valeur->getValeurData();
		 double value = data->getValue();
		lua_pushnumber(L, value);
	}
	else {
		cerr << endl << "PluginDataValeurProxy:getValue Type de valeur non-pris en compte" << endl << flush;
	}

	return 1;
}

/**
 * .
 *    - Return 1 :
 */
int PluginDataValeurProxy::setValue(lua_State *L) {
	if(dynamic_cast<ValeurFloat*>(_valeur)) {
		double value = lua_tonumber(L, -1);
		JktUtils::FloatData data(value);
		_valeur->setValeur(0, data);
	}
	else {
		cerr << endl << "PluginDataValeurProxy:setValue Type de valeur non-pris en compte" << endl << flush;
	}

	return 0;
}

/**
 * .
 *    - Return 1 :
 */
int PluginDataValeurProxy::getValeurFullId(lua_State *L) {
	vector<int> valeurBrancheId = _valeur->getBrancheId();
	int valeurId = _valeur->getValeurId();

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
 * .
 *    - Return 1 :
 */
int PluginDataValeurProxy::getBrancheId(lua_State *L) {
	vector<int> valeurBrancheId = _valeur->getBrancheId();

	lua_createtable(L, valeurBrancheId.size(), 0);

	for(int i=0; i<valeurBrancheId.size(); i++) {
		lua_pushinteger(L, valeurBrancheId.at(i));
		lua_rawseti (L, -2, i);
	}

	return 1;
}

} /* namespace JktPlugin */
