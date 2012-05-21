/*
 * AbstractPluginPanelProxy.h
 *
 *  Created on: 13 mai 2012
 *      Author: Erwin
 */

#ifndef ABSTRACTPLUGINPANELPROXY_H_
#define ABSTRACTPLUGINPANELPROXY_H_

namespace JktPlugin {

class PluginPanel;
class PluginContext;

class AbstractPluginPanelProxy {
	PluginPanel* _pluginPanel;
	PluginContext* _pluginContext;
public:
	AbstractPluginPanelProxy(lua_State* luaState);
	AbstractPluginPanelProxy(PluginContext* pluginContext);
	virtual ~AbstractPluginPanelProxy();

	void setPluginContext(PluginContext* pluginContext);
	void setPanel(PluginPanel* pluginPanel);

	// Enfants
	int addNotebook(lua_State* L);
	int addButton(lua_State* L);
	int addCheckbox(lua_State* L);
	int addBoxHoriz(lua_State* L);
	int addBoxVert(lua_State* L);
	int addNumeric(lua_State* L);
};

} /* namespace JktPlugin */
#endif /* ABSTRACTPLUGINPANELPROXY_H_ */
