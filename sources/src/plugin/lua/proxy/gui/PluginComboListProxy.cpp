/*
 * PluginComboListProxy.cpp
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#include <iostream>

using namespace std;

#include "plugin/lua/LuaUtils.h"
#include "plugin/PluginEngine.h"
#include "plugin/api/gui/PluginComboList.h"

#include "plugin/lua/proxy/gui/PluginComboListProxy.h"

namespace jkt {

const char PluginComboListProxy::className[] = "ComboList";

Lunar<PluginComboListProxy>::RegType PluginComboListProxy::methods[] = {
		{"addItem", &PluginComboListProxy::addItem},
		{"setItemSize", &PluginComboListProxy::setItemSize},
		{"selectItemByUserId", &PluginComboListProxy::selectItemByUserId},
		{"getSelectedItemUserId", &PluginComboListProxy::getSelectedItemUserId},

		{"expand", &PluginComboListProxy::expand},
		{"expandHoriz", &PluginComboListProxy::expandHoriz},
		{"expandVert", &PluginComboListProxy::expandVert},
		{0}
};

PluginComboListProxy::PluginComboListProxy(lua_State* L) {
	_pluginContext = NULL;
	_pluginComboList = NULL;
}

PluginComboListProxy::PluginComboListProxy(PluginContext* pluginContext) {
	_pluginContext = pluginContext;
	_pluginComboList = NULL;
}

PluginComboListProxy::~PluginComboListProxy() {
}

int PluginComboListProxy::push(lua_State* L) {
	return Lunar<PluginComboListProxy>::push(L, this);
}

void PluginComboListProxy::setWrappedObject(PluginComboList* pluginComboList) {
	_pluginComboList = pluginComboList;
}

/**
 * Add an item in the combo list.
 *    - Param 1 : Name of the item
 *    - Param 2 : Any number associated to the item and choose by the user
 */
int PluginComboListProxy::addItem(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 2, LUA_PARAM_STRING, LUA_PARAM_NUMBER)) {
		const char* value = lua_tostring(L, 1);
		int userId = lua_tonumber(L, 2);

		_pluginComboList->addItem(value, userId);
	}

	return 0;
}

/**
 * Select an item of the combo which is the first which the user id is the requested one.
 *    - Param 1 : Number associated to the item and choose by the user
 */
int PluginComboListProxy::selectItemByUserId(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 1, LUA_PARAM_NUMBER)) {
		int userId = lua_tonumber(L, 1);

		_pluginComboList->selectItemByUserId(userId);
	}

	return 0;
}

/**
 * Return the user identifier associated to the item which is actually selected in the combo list.
 *    - Return 1 : Number associated to the item and choose by the user
 */
int PluginComboListProxy::getSelectedItemUserId(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 0)) {
		_pluginComboList->getSelectedItemUserId();
	}

	return 0;
}

/**
 * Set the size of the items shown in the combo.
 *    - Param 1 : Mask of the format like 123456 to have 6 characters
 */
int PluginComboListProxy::setItemSize(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 1, LUA_PARAM_STRING)) {
		const char* mask = lua_tostring(L, 1);
		_pluginComboList->setComboSize(mask);
	}

	return 0;
}

/**
 * Expand the widget.
 */
int PluginComboListProxy::expand(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 0)) {
		_pluginComboList->expand();
	}

	return 0;
}

/**
 * Expand the widget horizontally.
 */
int PluginComboListProxy::expandHoriz(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 0)) {
		_pluginComboList->expandHoriz();
	}

	return 0;
}

/**
 * Expand the widget vertically.
 */
int PluginComboListProxy::expandVert(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 0)) {
		_pluginComboList->expandVert();
	}

	return 0;
}

void PluginComboListProxy::selectionChangedEvent() {
	lua_getglobal(_pluginContext->getLuaState(), "eventManager");

	// on vérifie si la fonction existe bien
	if (lua_isfunction(_pluginContext->getLuaState(), -1)) {
		Lunar<PluginComboListProxy>::push(_pluginContext->getLuaState(), this);

		int status = lua_pcall(_pluginContext->getLuaState(), 1, 0, 0);
		_pluginContext->logLuaError(status);
	}
}


} /* namespace jkt */
