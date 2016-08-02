/*
 * PluginEventProxy.h
 *
 *  Created on: 10 févr. 2014
 *      Author: vgdj7997
 */

#ifndef PLUGINEVENTPROXY_H_
#define PLUGINEVENTPROXY_H_

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "lunar.h"

#include "menu/Controller.h"
#include "plugin/PluginActionEvent.h"
#include <plugin/PluginEventSource.h>
#include "plugin/lua/proxy/LunarProxy.h"

namespace jkt {

class PluginEventProxy : public LunarProxy {
public:
	enum PLUGIN_EVENT_TYPE {
		UNKNOWN_EVENT = 0,
		MESSAGE_EVENT = 1,
		WIDGET_EVENT = 3
	};

	static const char className[];
	static Lunar<PluginEventProxy>::RegType methods[];

private:
	PLUGIN_EVENT_TYPE _type;
	Controller::Action _messageType;
	LunarProxy* _source;

public:
	PluginEventProxy(const PluginActionEvent& event);
	PluginEventProxy(PluginEventSource* event);
	PluginEventProxy(lua_State* L);
	~PluginEventProxy();

	int push(lua_State* L);
	int getSource(lua_State* L);
	int getType(lua_State* L);
	int getActionId(lua_State* L);
};

} /* namespace jkt */

#endif /* PLUGINEVENTPROXY_H_ */
