/*
 * PluginGuiController.cpp
 *
 *  Created on: 19 mai 2012
 *      Author: Erwin
 */

#include "plugin/api/gui/PluginButton.h"
#include "plugin/api/gui/PluginComboList.h"

#include "plugin/api/gui/PluginGuiController.h"

namespace jkt {

PluginGuiController::PluginGuiController() {
}

PluginGuiController::~PluginGuiController() {
}

void PluginGuiController::catchButtonEvent(AG_Event* event) {
	PluginButton* pluginButton = (PluginButton*)AG_PTR(1);
	pluginButton->buttonPressedEvent(event);
}

void PluginGuiController::catchComboListEvent(AG_Event* event) {
	PluginComboList* pluginComboList = (PluginComboList*)AG_PTR(1);
	pluginComboList->selectionChangedEvent(event);
}

} /* namespace jkt */
