/*
 * PluginTab.cpp
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#include "plugin/api/gui/PluginTab.h"

using namespace std;

namespace JktPlugin {

PluginTab::PluginTab(AG_Notebook* parent, const string& tabTitle) {
	_tab = AG_NotebookAddTab(parent, tabTitle.c_str(), AG_BOX_HORIZ);
}

PluginTab::~PluginTab() {
}


AG_Widget* PluginTab::getParent() {
	return (AG_Widget*)_tab;
}

} /* namespace JktPlugin */
