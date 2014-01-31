/*
 * PluginPanel.h
 *
 *  Created on: 13 mai 2012
 *      Author: Erwin
 */

#ifndef PLUGINPANEL_H_
#define PLUGINPANEL_H_

#include <string>

#include <agar/core.h>
#include <agar/gui.h>

namespace JktPlugin {

class PluginBox;
class PluginButton;
class PluginComboList;
class PluginCheckbox;
class PluginNotebook;
class PluginNumeric;
class ButtonPressedListener;

class PluginPanel {
public:
	PluginPanel();
	virtual ~PluginPanel();

	virtual AG_Widget* getParent() = 0;

	// Enfants
	PluginNotebook* addNotebook();
	PluginCheckbox* addCheckbox(const std::string& checkboxText);
	PluginButton* addButton(const std::string& buttonText, ButtonPressedListener* buttonPressedListener);
	PluginComboList* addComboList(const std::string& title);
	PluginBox* addBoxHoriz();
	PluginBox* addBoxVert();
	PluginNumeric* addNumeric(const std::string& numericText, const std::string& numericUnite);
};

} /* namespace JktPlugin */
#endif /* PLUGINPANEL_H_ */
