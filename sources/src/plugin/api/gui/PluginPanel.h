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
class PluginLabel;
class PluginComboList;
class PluginCheckbox;
class PluginNotebook;
class PluginNumeric;
class ButtonPressedListener;
class SelectionChangedListener;

class PluginPanel {
public:
	PluginPanel();
	virtual ~PluginPanel();

	virtual AG_Widget* getParent() = 0;

	// Widgets enfants

	PluginButton* addButton(const std::string& buttonText, ButtonPressedListener* buttonPressedListener);
	PluginCheckbox* addCheckbox(const std::string& checkboxText);
	PluginComboList* addComboList(const std::string& title, SelectionChangedListener* selectionChangedListener);
	PluginLabel* addLabel(const std::string& buttonText);
	PluginNotebook* addNotebook();
	PluginNumeric* addNumeric(const std::string& numericText, const std::string& numericUnite);
	void addSeparator();

	// Containers enfants
	PluginBox* addBoxHoriz();
	PluginBox* addBoxVert();

	// Opérations
	void expand();
	void expandHoriz();
	void expandVert();
};

} /* namespace JktPlugin */
#endif /* PLUGINPANEL_H_ */
