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

#include "data/ValeurInt.h"
#include "data/ValeurFloat.h"
#include "data/ValeurString.h"
#include "data/DataTree.h"
#include "main/Fabrique.h"
#include "data/ValeurFloat.h"
#include "plugin/lua/LuaUtils.h"
#include "plugin/PluginEngine.h"
#include "main/Game.h"

#include "plugin/lua/proxy/data/PluginDataValeurProxy.h"

namespace jkt {

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
	_valeur = 0;
}

/**
 * .
 *    - Return 1 :
 */
int PluginDataValeurProxy::getValue(lua_State *L) {
	if(dynamic_cast<ValeurInt*>(_valeur)) {
		int value = ((ValeurInt*)_valeur)->getValeur();
		lua_pushnumber(L, value);
	}
	else if(dynamic_cast<ValeurFloat*>(_valeur)) {
		double value = ((ValeurFloat*)_valeur)->getValeur();
		lua_pushnumber(L, value);
	}
	else if(dynamic_cast<ValeurString*>(_valeur)) {
		string value = ((ValeurString*)_valeur)->getValeur();
		lua_pushstring(L, value.c_str());
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
		((ValeurFloat*)_valeur)->updateValeur(value);
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

	LuaUtils::pushIntArray(L, valeurBrancheId, valeurId);

	return 1;
}

/**
 * .
 *    - Return 1 :
 */
int PluginDataValeurProxy::getBrancheId(lua_State *L) {
	vector<int> brancheId = _valeur->getBrancheId();

	LuaUtils::pushIntArray(L, brancheId);

	return 1;
}

} /* namespace jkt */
