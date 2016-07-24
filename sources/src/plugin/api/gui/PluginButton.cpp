/*
 * PluginButton.cpp
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#include "plugin/api/gui/PluginGuiController.h"

#include "plugin/api/gui/PluginButton.h"

using namespace std;

namespace jkt {

PluginButton::PluginButton(AG_Widget* parent, const string& buttonText, ButtonPressedListener* buttonPressedListener) {
	_button = AG_ButtonNewFn(parent, 0, buttonText.c_str(), &PluginGuiController::catchButtonEvent, "%p", this);
	_buttonPressedListener = buttonPressedListener;
}

PluginButton::~PluginButton() {
}

void PluginButton::buttonPressedEvent(AG_Event* event) {
	_buttonPressedListener->buttonPressedEvent();
}

void PluginButton::expand() {
	AG_Expand(_button);
}

void PluginButton::expandHoriz() {
	AG_ExpandHoriz(_button);
}

void PluginButton::expandVert() {
	AG_ExpandVert(_button);
}

} /* namespace jkt */
