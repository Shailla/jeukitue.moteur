/*
 * PluginGuiController.cpp
 *
 *  Created on: 19 mai 2012
 *      Author: Erwin
 */

#include "plugin/api/gui/PluginButton.h"

#include "plugin/api/gui/PluginGuiController.h"

namespace JktPlugin {

PluginGuiController::PluginGuiController() {
}

PluginGuiController::~PluginGuiController() {
}

void PluginGuiController::catchButtonEvents(AG_Event* event) {
	PluginButton* pluginButton = (PluginButton*)AG_PTR(1);
	pluginButton->buttonPressedEvent(event);
}

} /* namespace JktPlugin */
