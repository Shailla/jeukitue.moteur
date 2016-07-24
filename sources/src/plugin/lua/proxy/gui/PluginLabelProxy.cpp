/*
 * PluginLabelProxy.cpp
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#include <iostream>

using namespace std;

#include "plugin/lua/LuaUtils.h"
#include "plugin/PluginWidgetEvent.h"
#include "plugin/PluginContext.h"
#include "plugin/PluginEngine.h"
#include "plugin/api/gui/PluginLabel.h"

#include "plugin/lua/proxy/gui/PluginLabelProxy.h"

namespace jkt {

const char PluginLabelProxy::className[] = "Label";

Lunar<PluginLabelProxy>::RegType PluginLabelProxy::methods[] = {
		{"expand", &PluginLabelProxy::expand},
		{"expandHoriz", &PluginLabelProxy::expandHoriz},
		{"expandVert", &PluginLabelProxy::expandVert},
		{0}
};

PluginLabelProxy::PluginLabelProxy(lua_State* L) {
	_pluginContext = NULL;
	_pluginLabel = NULL;
}

PluginLabelProxy::PluginLabelProxy(PluginContext* pluginContext) {
	_pluginContext = pluginContext;
	_pluginLabel = NULL;
}

PluginLabelProxy::~PluginLabelProxy() {
}

int PluginLabelProxy::push(lua_State* L) {
	return Lunar<PluginLabelProxy>::push(L, this);
}

void PluginLabelProxy::setWrappedObject(PluginLabel* pluginLabel) {
	_pluginLabel = pluginLabel;
}

/**
 * Expand the widget.
 */
int PluginLabelProxy::expand(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 0)) {
		_pluginLabel->expand();
	}

	return 0;
}

/**
 * Expand the widget horizontally.
 */
int PluginLabelProxy::expandHoriz(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 0)) {
		_pluginLabel->expandHoriz();
	}

	return 0;
}

/**
 * Expand the widget vertically.
 */
int PluginLabelProxy::expandVert(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 0)) {
		_pluginLabel->expandVert();
	}

	return 0;
}

} /* namespace jkt */
