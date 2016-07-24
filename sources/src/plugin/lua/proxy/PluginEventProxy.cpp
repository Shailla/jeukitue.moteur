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
		{"getSource", &PluginEventProxy::getSource},
		{"getType", &PluginEventProxy::getType},
		{"getActionId", &PluginEventProxy::getActionId},
		{0}
};

PluginEventProxy::PluginEventProxy(const PluginActionEvent& event) {
	_type = MESSAGE_EVENT;
	_messageType = event.getActionId();
	_source = 0;
}

PluginEventProxy::PluginEventProxy(PluginWidgetEvent* event) {
	_type = WIDGET_EVENT;
	_messageType = Controller::Action::Unknown;
	_source = event->getSource();
}

PluginEventProxy::PluginEventProxy(lua_State* L) {
	_type = UNKNOWN_EVENT;
	_messageType = Controller::Action::Unknown;
	_source = 0;
}

PluginEventProxy::~PluginEventProxy() {
}

int PluginEventProxy::push(lua_State* L) {
	return Lunar<PluginEventProxy>::push(L, this);
}

int PluginEventProxy::getType(lua_State* L) {
	lua_pushnumber(L, _type);

	return 1;
}

int PluginEventProxy::getActionId(lua_State* L) {
	lua_pushnumber(L, _messageType);

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

} /* namespace jkt */
