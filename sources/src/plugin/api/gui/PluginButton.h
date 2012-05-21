/*
 * PluginButton.h
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#ifndef PLUGINBUTTON_H_
#define PLUGINBUTTON_H_

#include <string>

#include <agar/core.h>
#include <agar/gui.h>

#include "plugin/api/gui/listener/ButtonPressedListener.h"
#include "plugin/PluginContext.h"

namespace JktPlugin {

class PluginButton {
	ButtonPressedListener* _buttonPressedListener;
	AG_Button* _button;
public:
	PluginButton(AG_Widget* parent, const std::string& buttonText, ButtonPressedListener* buttonPressedListener);
	virtual ~PluginButton();

	void buttonPressedEvent(AG_Event* event);
};

} /* namespace JktPlugin */
#endif /* PLUGINBUTTON_H_ */
