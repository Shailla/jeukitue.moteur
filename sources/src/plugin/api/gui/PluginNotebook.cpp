/*
 * PluginNotebook.cpp
 *
 *  Created on: 13 mai 2012
 *      Author: Erwin
 */

#include "plugin/api/gui/PluginTab.h"

#include "plugin/api/gui/PluginNotebook.h"

using namespace std;

namespace jkt {

PluginNotebook::PluginNotebook(AG_Widget* parent) {
	_notebook = AG_NotebookNew(parent, 0);
	AG_Expand(_notebook);
}

PluginTab* PluginNotebook::addTab(const string& tabTitle) {
	return new PluginTab(_notebook, tabTitle);

}

PluginNotebook::~PluginNotebook() {
}

} /* namespace jkt */
