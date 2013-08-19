/*
 * PluginNotebookProxy.h
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#ifndef PLUGINNOTEBOOKPROXY_H_
#define PLUGINNOTEBOOKPROXY_H_

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#include "lunaFive.h"

namespace JktPlugin {

class PluginContext;
class PluginNotebook;

class PluginNotebookProxy {
	friend class Luna<PluginNotebookProxy>;
	static const char *className;
	static const Luna<PluginNotebookProxy>::FunctionType methods[];
	static const Luna<PluginNotebookProxy>::PropertyType properties[];

	bool isExisting; // This is used by Luna to see whether it's been created by createFromExisting.  Don't set it.
	bool isPrecious; // This is used to tell Luna not to garbage collect the object, in case other objects might reference it.  Set it in your classes constructor.

	PluginContext* _pluginContext;
	PluginNotebook* _pluginNotebook;
public:
	PluginNotebookProxy(lua_State* L);
	PluginNotebookProxy(PluginContext* pluginContext, PluginNotebook* pluginNotebook);
	virtual ~PluginNotebookProxy();

	// Enfants
	int addTab(lua_State* L);
};

} /* namespace JktPlugin */
#endif /* PLUGINNOTEBOOKPROXY_H_ */
