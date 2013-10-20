/*
 * AbstractPluginPanelProxy.cpp
 *
 *  Created on: 13 mai 2012
 *      Author: Erwin
 */

#include "plugin/lua/proxy/gui/PluginButtonProxy.h"
#include "plugin/lua/proxy/gui/PluginBoxProxy.h"
#include "plugin/lua/proxy/gui/PluginCheckboxProxy.h"
#include "plugin/lua/proxy/gui/PluginNotebookProxy.h"
#include "plugin/lua/proxy/gui/PluginNumericProxy.h"
#include "plugin/PluginContext.h"
#include "plugin/lua/LuaUtils.h"

#include "plugin/lua/proxy/gui/AbstractPluginPanelProxy.h"

namespace JktPlugin {

AbstractPluginPanelProxy::AbstractPluginPanelProxy(lua_State* luaState) {
	_pluginContext = NULL;
	_pluginPanel = NULL;
}

AbstractPluginPanelProxy::AbstractPluginPanelProxy(PluginContext* pluginContext) {
	_pluginContext = pluginContext;
	_pluginPanel = NULL;
}

void AbstractPluginPanelProxy::setPluginContext(PluginContext* pluginContext) {
	_pluginContext = pluginContext;
}

void AbstractPluginPanelProxy::setPanel(PluginPanel* pluginPanel) {
	_pluginPanel = pluginPanel;
}

AbstractPluginPanelProxy::~AbstractPluginPanelProxy() {
}

/**
 * Mettre des onglets dans la fenêtre.
 *    - Return 1 : container de type NoteBook
 */
int AbstractPluginPanelProxy::addNotebook(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 1, LUA_PARAM_USERDATA)) {
		PluginNotebookProxy* notebookProxy = new PluginNotebookProxy(_pluginContext, _pluginPanel->addNotebook());
		Luna<PluginNotebookProxy>::push(L, notebookProxy);
	}

	return 1;
}

/**
 * Ajoute un bouton de type case à cocher dans le panel.
 *    - Param 1 : texte affiché avec le bouton
 *    - Return 1 : objet de type Checkbox
 */
int AbstractPluginPanelProxy::addCheckbox(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 2, LUA_PARAM_USERDATA, LUA_PARAM_STRING)) {
		const char* checkboxText = lua_tostring(L, 2);
		PluginCheckboxProxy* checkboxProxy = new PluginCheckboxProxy(_pluginPanel->addCheckbox(checkboxText));
		Luna<PluginCheckboxProxy>::push(L, checkboxProxy);
	}

	return 1;
}

/**
 * Ajoute un bouton de type case à cocher dans le panel.
 *    - Param 1 : texte affiché avec le bouton
 *    - Param 2 : fonction Lua de callback déclenchée quand le bouton est activé
 *    - Return 1 : objet de type Checkbox
 */
int AbstractPluginPanelProxy::addButton(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 2, LUA_PARAM_USERDATA, LUA_PARAM_STRING)) {
		const char* buttonText = lua_tostring(L, 2);

		PluginButtonProxy* buttonProxy = new PluginButtonProxy(_pluginContext);
		PluginButton* pluginButton = _pluginPanel->addButton(buttonText, buttonProxy);
		buttonProxy->setWrappedObject(pluginButton);

		Luna<PluginButtonProxy>::push(L, buttonProxy);
	}

	return 1;
}

/**
 * Ajoute un panel horizontal.
 *    - Return 1 : container de type Box
 */
int AbstractPluginPanelProxy::addBoxHoriz(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 1, LUA_PARAM_USERDATA)) {
		PluginBoxProxy* boxProxy = new PluginBoxProxy(_pluginContext, _pluginPanel->addBoxHoriz());
		Luna<PluginBoxProxy>::push(L, boxProxy);
	}

	return 1;
}

/**
 * Ajoute un panel vertical.
 *    - Return 1 : container de type Box
 */
int AbstractPluginPanelProxy::addBoxVert(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 1, LUA_PARAM_USERDATA)) {
		PluginBoxProxy* boxProxy = new PluginBoxProxy(_pluginContext, _pluginPanel->addBoxVert());
		Luna<PluginBoxProxy>::push(L, boxProxy);
	}

	return 1;
}

/**
 * Ajoute un champ de saisie d'une valeur flottante avec boutons +/-.
 *    - Param 1 : texte descriptif de la valeur
 *    - Param 2 : unité de la valeur
 *    - Return 1 : objet de type Numeric
 */
int AbstractPluginPanelProxy::addNumeric(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 3, LUA_PARAM_USERDATA, LUA_PARAM_STRING, LUA_PARAM_STRING)) {
		const char* numericText = lua_tostring(L, 2);
		const char* numericUnite = lua_tostring(L, 3);
		PluginNumericProxy* numericProxy = new PluginNumericProxy(_pluginPanel->addNumeric(numericText, numericUnite));
		Luna<PluginNumericProxy>::push(L, numericProxy);
	}

	return 1;
}

} /* namespace JktPlugin */
