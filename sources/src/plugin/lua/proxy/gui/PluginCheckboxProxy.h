/*
 * PluginCheckboxProxy.h
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#ifndef PLUGINCHECKBOXPROXY_H_
#define PLUGINCHECKBOXPROXY_H_

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "lunaFive.h"

#include "plugin/api/gui/PluginCheckbox.h"

namespace JktPlugin {

class PluginCheckboxProxy {
	friend class Luna<PluginCheckboxProxy>;
	static const char *className;
	static const Luna<PluginCheckboxProxy>::FunctionType methods[];
	static const Luna<PluginCheckboxProxy>::PropertyType properties[];

	bool isExisting; // This is used by Luna to see whether it's been created by createFromExisting.  Don't set it.
	bool isPrecious; // This is used to tell Luna not to garbage collect the object, in case other objects might reference it.  Set it in your classes constructor.

	PluginCheckbox* _pluginCheckbox;
public:
	PluginCheckboxProxy(lua_State* L);
	PluginCheckboxProxy(PluginCheckbox* pluginCheckbox);
	virtual ~PluginCheckboxProxy();

	int getValue(lua_State* L);
	int setValue(lua_State* L);
};

} /* namespace JktPlugin */
#endif /* PLUGINCHECKBOXPROXY_H_ */
