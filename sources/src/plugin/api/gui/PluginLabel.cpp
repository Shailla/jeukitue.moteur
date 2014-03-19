/*
 * PluginLabel.cpp
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#include "plugin/api/gui/PluginGuiController.h"

#include "plugin/api/gui/PluginLabel.h"

using namespace std;

namespace JktPlugin {

PluginLabel::PluginLabel(AG_Widget* parent, const string& labelText) {
	_label = AG_LabelNew(parent, 0, labelText.c_str());
}

PluginLabel::~PluginLabel() {
}

void PluginLabel::expand() {
	AG_Expand(_label);
}

void PluginLabel::expandHoriz() {
	AG_ExpandHoriz(_label);
}

void PluginLabel::expandVert() {
	AG_ExpandVert(_label);
}

} /* namespace JktPlugin */
