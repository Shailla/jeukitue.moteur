/*
 * LuaWindowProxy.cpp
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#include <iostream>

using namespace std;

#include "plugin/api/gui/LuaWindowProxy.h"

namespace JktPlugin {

const char LuaWindowProxy::className[] = "Window";

Lunar<LuaWindowProxy>::RegType LuaWindowProxy::methods[] = {
		{"setSize", &LuaWindowProxy::setSize},
		{"setTitle", &LuaWindowProxy::setTitle},
		{"show", &LuaWindowProxy::show},
		{"hide", &LuaWindowProxy::hide},
		{0, 0}
};

LuaWindowProxy::LuaWindowProxy(lua_State* L) : LuaWindow() {
	int argCount = lua_gettop(L);

	if(argCount != 0) {
		cerr << endl << "Erreur de paramètre LUA";
	}
}

LuaWindowProxy::~LuaWindowProxy() {
}

/**
 * Modifier les dimensions de la fenêtre.
 *    - Param 1 : Taille sur l'axe horizontal
 *    - Param 2 : Taille sur l'axe vertical
 */
int LuaWindowProxy::setSize(lua_State* L) {
	int argCount = lua_gettop(L);

	if(argCount == 2 && lua_isnumber(L, 1) && lua_isnumber(L, 2)) {
		const int x = lua_tonumber(L, 1);
		const int y = lua_tonumber(L, 2);
		LuaWindow::setSize(x, y);
	}
	else {
		cerr << endl << "Erreur de paramètre LUA";
	}

	return 0;
}

/**
 * Modifier le titre de la fenêtre.
 */
int LuaWindowProxy::setTitle(lua_State* L) {
	int argCount = lua_gettop(L);

	if(argCount == 1 && lua_isstring(L, 1)) {
		const char* title = lua_tostring(L, 1);
		LuaWindow::setTitle(title);
	}
	else {
		cerr << endl << "Erreur de paramètre LUA";
	}

	return 0;
}

/**
 * Afficher la fenêtre.
 */
int LuaWindowProxy::show(lua_State* L) {
	int argCount = lua_gettop(L);

	if(argCount == 0) {
		LuaWindow::show();
	}
	else {
		cerr << endl << "Erreur de paramètre LUA";
	}

	return 0;
}

/**
 * Afficher la fenêtre.
 */
int LuaWindowProxy::hide(lua_State* L) {
	int argCount = lua_gettop(L);

	if(argCount == 0) {
		LuaWindow::hide();
	}
	else {
		cerr << endl << "Erreur de paramètre LUA";
	}

	return 0;
}

} /* namespace JktPlugin */
