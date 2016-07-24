/*
 * PluginBox.cpp
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#include "plugin/api/gui/PluginBox.h"

using namespace std;

namespace jkt {

PluginBox::PluginBox(AG_Widget* parent, ag_box_type checkboxType) {
	_box = AG_BoxNew(parent, checkboxType, AG_BOX_HOMOGENOUS);
}

PluginBox::~PluginBox() {
}


AG_Widget* PluginBox::getParent() {
	return (AG_Widget*)_box;
}

} /* namespace jkt */
