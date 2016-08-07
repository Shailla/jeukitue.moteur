/*
 * PluginEventProxy.cpp
 *
 *  Created on: 10 févr. 2014
 *      Author: vgdj7997
 */

#include "menu/Controller.h"

#include "plugin/lua/proxy/PluginEventProxy.h"

namespace jkt {

const char PluginEventProxy::className[] = "Event";

Lunar<PluginEventProxy>::RegType PluginEventProxy::methods[] = {
		{"getActionId", &PluginEventProxy::getActionId},
		{"getSource", &PluginEventProxy::getSource},
		{"getInfo", &PluginEventProxy::getInfo},
		{0}
};

PluginEventProxy::PluginEventProxy(int actionId) {
	_actionId = actionId;
	_source = 0;
	_info = 0;
}

PluginEventProxy::PluginEventProxy(int actionId, LunarProxy* source, LunarProxy* info) {
	_actionId = actionId;
	_source = source;
	_info = info;
}

PluginEventProxy::PluginEventProxy(lua_State* L) {	// UNKNOWN_EVENT
	_actionId = Controller::Action::Unknown;
	_source = 0;
	_info = 0;
}

PluginEventProxy::~PluginEventProxy() {
}

int PluginEventProxy::push(lua_State* L) {
	return Lunar<PluginEventProxy>::push(L, this);
}

int PluginEventProxy::getActionId(lua_State* L) {
	lua_pushnumber(L, _actionId);

	return 1;
}

int PluginEventProxy::getSource(lua_State* L) {
	int result;

	if(_source) {
		result = _source->push(L);
	}
	else {
		result = 0;
	}

	return result;
}

void PluginEventProxy::setInfo(LunarProxy* info) {
	_info = info;
}

int PluginEventProxy::getInfo(lua_State* L) {
	int result;

	if(_info) {
		result = _info->push(L);
	}
	else {
		result = 0;
	}

	return result;
}

} /* namespace jkt */
