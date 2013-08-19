/*
 * PluginButtonProxy.h
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#ifndef PLUGINBUTTONPROXY_H_
#define PLUGINBUTTONPROXY_H_

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "lunaFive.h"

#include "plugin/api/gui/listener/ButtonPressedListener.h"

namespace JktPlugin {

class PluginContext;
class PluginButton;

class PluginButtonProxy : public ButtonPressedListener {
	friend class Luna<PluginButtonProxy>;
	static const char *className;
	static const Luna<PluginButtonProxy>::FunctionType methods[];
	static const Luna<PluginButtonProxy>::PropertyType properties[];

	bool isExisting; // This is used by Luna to see whether it's been created by createFromExisting.  Don't set it.
	bool isPrecious; // This is used to tell Luna not to garbage collect the object, in case other objects might reference it.  Set it in your classes constructor.

	PluginContext* _pluginContext;
	PluginButton* _pluginButton;
public:
	PluginButtonProxy(lua_State* L);
	PluginButtonProxy(PluginContext* pluginContext);
	virtual ~PluginButtonProxy();

	void setWrappedObject(PluginButton* pluginButton);
	void buttonPressedEvent();
};

} /* namespace JktPlugin */
#endif /* PLUGINBUTTONPROXY_H_ */
