/*
 * PluginButton.cpp
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#include "plugin/api/gui/PluginGuiController.h"

#include "plugin/api/gui/PluginButton.h"

using namespace std;

namespace JktPlugin {

PluginButton::PluginButton(AG_Widget* parent, const string& buttonText, ButtonPressedListener* buttonPressedListener) {
	_buttonPressedListener = buttonPressedListener;
	_button = AG_ButtonNewFn(parent, 0, buttonText.c_str(), &PluginGuiController::catchButtonEvents, "%p", this);
}

PluginButton::~PluginButton() {
}

void PluginButton::buttonPressedEvent(AG_Event* event) {
	_buttonPressedListener->buttonPressedEvent();
}

} /* namespace JktPlugin */
