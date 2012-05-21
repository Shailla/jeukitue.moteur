/*
 * PluginRadioButton.cpp
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#include "plugin/api/gui/PluginCheckbox.h"

using namespace std;

namespace JktPlugin {

PluginCheckbox::PluginCheckbox(AG_Widget* parent, const string& checkboxText) {
	_checkbox = AG_CheckboxNewInt(parent, AG_RADIO_HFILL, checkboxText.c_str(), &_value);
}

PluginCheckbox::~PluginCheckbox() {
}

bool PluginCheckbox::getValue() {
	return _value;
}

void PluginCheckbox::setValue(const bool value) {
	_value = value;
}

} /* namespace JktPlugin */
