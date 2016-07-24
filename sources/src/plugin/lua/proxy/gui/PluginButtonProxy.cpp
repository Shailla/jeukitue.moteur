/*
 * PluginButtonProxy.cpp
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
#include "plugin/api/gui/PluginButton.h"

#include "plugin/lua/proxy/gui/PluginButtonProxy.h"

namespace jkt {

const char PluginButtonProxy::className[] = "Button";

Lunar<PluginButtonProxy>::RegType PluginButtonProxy::methods[] = {
		{"expand", &PluginButtonProxy::expand},
		{"expandHoriz", &PluginButtonProxy::expandHoriz},
		{"expandVert", &PluginButtonProxy::expandVert},
		{0}
};

PluginButtonProxy::PluginButtonProxy(lua_State* L) {
	_pluginContext = NULL;
	_pluginButton = NULL;
}

PluginButtonProxy::PluginButtonProxy(PluginContext* pluginContext) {
	_pluginContext = pluginContext;
	_pluginButton = NULL;
}

PluginButtonProxy::~PluginButtonProxy() {
}

int PluginButtonProxy::push(lua_State* L) {
	return Lunar<PluginButtonProxy>::push(L, this);
}

void PluginButtonProxy::buttonPressedEvent() {
	PluginWidgetEvent* event = new PluginWidgetEvent(this);
	_pluginContext->dispatchEvent(event);
}

void PluginButtonProxy::setWrappedObject(PluginButton* pluginButton) {
	_pluginButton = pluginButton;
}

/**
 * Expand the widget.
 */
int PluginButtonProxy::expand(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 0)) {
		_pluginButton->expand();
	}

	return 0;
}

/**
 * Expand the widget horizontally.
 */
int PluginButtonProxy::expandHoriz(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 0)) {
		_pluginButton->expandHoriz();
	}

	return 0;
}

/**
 * Expand the widget vertically.
 */
int PluginButtonProxy::expandVert(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 0)) {
		_pluginButton->expandVert();
	}

	return 0;
}

} /* namespace jkt */
