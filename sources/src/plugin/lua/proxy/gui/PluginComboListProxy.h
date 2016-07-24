/*
 * PluginComboListProxy.h
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#ifndef PLUGINCOMBOLISTPROXY_H_
#define PLUGINCOMBOLISTPROXY_H_

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "lunar.h"

#include "plugin/lua/proxy/LunarProxy.h"
#include "plugin/api/gui/listener/SelectionChangedListener.h"

namespace jkt {

class PluginContext;
class PluginComboList;

class PluginComboListProxy : public LunarProxy, public SelectionChangedListener {
public:
	static const char className[];
	static Lunar<PluginComboListProxy>::RegType methods[];

private:
	PluginContext* _pluginContext;
	PluginComboList* _pluginComboList;

public:
	PluginComboListProxy(lua_State* L);
	PluginComboListProxy(PluginContext* pluginContext);
	virtual ~PluginComboListProxy();

	int push(lua_State* L);
	void setWrappedObject(PluginComboList* pluginComboList);

	int addItem(lua_State* L);
	int setItemSize(lua_State* L);

	int selectItemByUserId(lua_State* L);
	int getSelectedItemUserId(lua_State* L);

	int expand(lua_State* L);
	int expandHoriz(lua_State* L);
	int expandVert(lua_State* L);

	void selectionChangedEvent();
};

} /* namespace jkt */
#endif /* PLUGINCOMBOLISTPROXY_H_ */
