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
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 0)) {
		PluginNotebookProxy* notebookProxy = new PluginNotebookProxy(_pluginContext, _pluginPanel->addNotebook());
		Lunar<PluginNotebookProxy>::push(L, notebookProxy, false);
	}

	return 1;
}

/**
 * Ajoute un bouton de type case à cocher dans le panel.
 *    - Param 1 : texte affiché avec le bouton
 *    - Return 1 : objet de type Checkbox
 */
int AbstractPluginPanelProxy::addCheckbox(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 1, LUA_PARAM_STRING)) {
		const char* checkboxText = lua_tostring(L, 1);
		PluginCheckboxProxy* checkboxProxy = new PluginCheckboxProxy(_pluginPanel->addCheckbox(checkboxText));
		Lunar<PluginCheckboxProxy>::push(L, checkboxProxy, false);
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
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 1, LUA_PARAM_STRING)) {
		const char* buttonText = lua_tostring(L, 1);

		PluginButtonProxy* buttonProxy = new PluginButtonProxy(_pluginContext);
		PluginButton* pluginButton = _pluginPanel->addButton(buttonText, buttonProxy);
		buttonProxy->setWrappedObject(pluginButton);

		Lunar<PluginButtonProxy>::push(L, buttonProxy, false);
	}

	return 1;
}

/**
 * Ajoute un panel horizontal.
 *    - Return 1 : container de type Box
 */
int AbstractPluginPanelProxy::addBoxHoriz(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 0)) {
		PluginBoxProxy* boxProxy = new PluginBoxProxy(_pluginContext, _pluginPanel->addBoxHoriz());
		Lunar<PluginBoxProxy>::push(L, boxProxy, false);
	}

	return 1;
}

/**
 * Ajoute un panel vertical.
 *    - Return 1 : container de type Box
 */
int AbstractPluginPanelProxy::addBoxVert(lua_State* L) {
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 0)) {
		PluginBoxProxy* boxProxy = new PluginBoxProxy(_pluginContext, _pluginPanel->addBoxVert());
		Lunar<PluginBoxProxy>::push(L, boxProxy, false);
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
	if(LuaUtils::isCheckLuaParametersTypes(L, __FILE__, __FUNCTION__, 2, LUA_PARAM_STRING, LUA_PARAM_STRING)) {
		const char* numericText = lua_tostring(L, 1);
		const char* numericUnite = lua_tostring(L, 1);
		PluginNumericProxy* numericProxy = new PluginNumericProxy(_pluginPanel->addNumeric(numericText, numericUnite));
		Lunar<PluginNumericProxy>::push(L, numericProxy, false);
	}

	return 1;
}

} /* namespace JktPlugin */
