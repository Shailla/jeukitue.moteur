/*
 * PluginDataTreeProxy.cpp
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#include <iostream>
#include <sstream>

using namespace std;

#include "util/Trace.h"
#include "data/DataTree.h"
#include "main/Fabrique.h"
#include "plugin/lua/LuaUtils.h"
#include "main/Game.h"
#include "plugin/lua/proxy/data/PluginDataTreeProxy.h"
#include "plugin/lua/proxy/game/PluginPlayerProxy.h"
#include "plugin/lua/proxy/map/PluginMapProxy.h"

#include "plugin/lua/proxy/game/PluginGameProxy.h"

extern CGame Game;			// Contient toutes les données vivantes du jeu

namespace jkt {

const char PluginGameProxy::className[] = "Game";

Lunar<PluginGameProxy>::RegType PluginGameProxy::methods[] = {
		{"getMap", &PluginGameProxy::getMap},
		{"getDataTree", &PluginGameProxy::getDataTree},
		{"getPlayer", &PluginGameProxy::getPlayer},
		{0}
};

PluginGameProxy::PluginGameProxy(CGame* game) {
	_game = game;
}

PluginGameProxy::PluginGameProxy(lua_State* L) {
	_game = 0;
}

int PluginGameProxy::getGame(lua_State *L) {
	return Lunar<PluginGameProxy>::push(L, new PluginGameProxy(&Game));
}

/**
 * Retourne l'arbre de données de la partie en cours.
 *    - Return 1 : PluginDataTreeProxy
 */
int PluginGameProxy::getDataTree(lua_State *L) {
	LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 0);

	if(_game) {
		DataTree* dataTree = _game->getDataTree();

		if(dataTree) {
			return Lunar<PluginDataTreeProxy>::push(L, new PluginDataTreeProxy(dataTree));
		}
	}

	return 0;
}

/**
 * Retourne le joueur en fonction de son identifiant.
 *    - Param 1  : Identifiant du joueur dans la partie
 *    - Return 1 : PluginPlayerProxy
 */
int PluginGameProxy::getPlayer(lua_State *L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 1, LUA_PARAM_NUMBER)) {
		int playerId = lua_tonumber(L, 1);

		if(_game) {
			CPlayer* player = _game->getPlayer(playerId);

			if(player) {
				Lunar<PluginPlayerProxy>::push(L, new PluginPlayerProxy(player));
				return 1;
			}
		}
	}

	return 0;
}

/**
 * Retourne la Map de la partie en cours.
 *    - Return 1 : PluginMapProxy
 */
int PluginGameProxy::getMap(lua_State *L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 0)) {

		if(_game) {
			CMap* map = _game->getMap();

			if(map) {
				return Lunar<PluginMapProxy>::push(L, new PluginMapProxy(map));
			}
		}
	}

	return 0;
}

int PluginGameProxy::push(lua_State* L) {
	return Lunar<PluginGameProxy>::push(L, this);
}

} /* namespace jkt */
