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

namespace jkt {

class PluginButton {
	AG_Button* _button;
	ButtonPressedListener* _buttonPressedListener;
public:
	PluginButton(AG_Widget* parent, const std::string& buttonText, ButtonPressedListener* buttonPressedListener);
	virtual ~PluginButton();

	void buttonPressedEvent(AG_Event* event);

	void expand();
	void expandHoriz();
	void expandVert();
};

} /* namespace jkt */
#endif /* PLUGINBUTTON_H_ */
