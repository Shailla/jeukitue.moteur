/*
 * PluginPanel.cpp
 *
 *  Created on: 13 mai 2012
 *      Author: Erwin
 */

#include "plugin/api/gui/PluginBox.h"
#include "plugin/api/gui/PluginButton.h"
#include "plugin/api/gui/PluginLabel.h"
#include "plugin/api/gui/PluginComboList.h"
#include "plugin/api/gui/PluginCheckbox.h"
#include "plugin/api/gui/PluginNotebook.h"
#include "plugin/api/gui/PluginNumeric.h"

#include "plugin/api/gui/PluginPanel.h"

using namespace std;

namespace jkt {

PluginPanel::PluginPanel() {

}

PluginPanel::~PluginPanel() {
}

PluginNotebook* PluginPanel::addNotebook() {
	PluginNotebook* pluginNotebook = new PluginNotebook(getParent());

	return pluginNotebook;
}

PluginCheckbox* PluginPanel::addCheckbox(const string& checkboxText) {
	return new PluginCheckbox(getParent(), checkboxText);
}

PluginButton* PluginPanel::addButton(const string& buttonText, ButtonPressedListener* buttonPressedListener) {
	return new PluginButton(getParent(), buttonText, buttonPressedListener);
}

PluginLabel* PluginPanel::addLabel(const string& labelText) {
	return new PluginLabel(getParent(), labelText);
}

PluginComboList* PluginPanel::addComboList(const string& title, SelectionChangedListener* selectionChangedListener) {
	return new PluginComboList(getParent(), title, selectionChangedListener);
}

PluginBox* PluginPanel::addBoxHoriz() {
	PluginBox* pluginBox = new PluginBox(getParent(), AG_BOX_HORIZ);

	return pluginBox;
}

PluginBox* PluginPanel::addBoxVert() {
	PluginBox* pluginBox = new PluginBox(getParent(), AG_BOX_VERT);

	return pluginBox;
}

PluginNumeric* PluginPanel::addNumeric(const std::string& numericText, const std::string& numericUnite) {
	return new PluginNumeric(getParent(), numericText, numericUnite);
}

void PluginPanel::addSeparator() {
	AG_SeparatorNew(getParent(), AG_SEPARATOR_HORIZ);
}

void PluginPanel::expand() {
	AG_Expand(getParent());
}

void PluginPanel::expandHoriz() {
	AG_ExpandHoriz(getParent());
}

void PluginPanel::expandVert() {
	AG_ExpandVert(getParent());
}

} /* namespace jkt */
