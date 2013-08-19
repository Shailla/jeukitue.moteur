/*
 * PluginTabProxy.h
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#ifndef PLUGINBOXPROXY_H_
#define PLUGINBOXPROXY_H_

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "lunaFive.h"

#include "plugin/api/gui/PluginBox.h"
#include "plugin/lua/proxy/gui/AbstractPluginPanelProxy.h"

namespace JktPlugin {

class PluginBoxProxy : public AbstractPluginPanelProxy {
	friend class Luna<PluginBoxProxy>;
	static const char className[];
	static Luna<PluginBoxProxy>::FunctionType methods[];

	PluginBox* _pluginBox;
public:
	PluginBoxProxy(lua_State* L);
	PluginBoxProxy(PluginContext* pluginContext, PluginBox* pluginBox);
	virtual ~PluginBoxProxy();
};

} /* namespace JktPlugin */
#endif /* PLUGINBOXPROXY_H_ */
