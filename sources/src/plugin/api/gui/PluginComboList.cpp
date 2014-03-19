/*
 * PluginComboList.cpp
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#include <agar/core/queue.h>

#include "plugin/api/gui/PluginGuiController.h"

#include "plugin/api/gui/PluginComboList.h"

using namespace std;

namespace JktPlugin {

PluginComboList::PluginComboList(AG_Widget* parent, const string& title, SelectionChangedListener* selectionChangedListener) {
	_selectionChangedListener = selectionChangedListener;
	_combo = AG_ComboNew(parent, 0, title.c_str());
}

PluginComboList::~PluginComboList() {
}

void PluginComboList::addItem(const string& itemName, int userId)  {
	AG_ObjectLock(_combo->list);

	AG_TlistAddPtr(_combo->list, NULL, itemName.c_str(), (void*)userId);

	AG_ObjectUnlock(_combo->list);
}

void PluginComboList::selectItemByUserId(int userId) {
	AG_ComboSelectPointer(_combo, (void*)userId);
}

int PluginComboList::getSelectedItemUserId()  {
	return (int)AG_TlistSelectedItemPtr(_combo->list);
}

void PluginComboList::setComboSize(const string& mask)  {
	AG_ComboSizeHint(_combo, mask.c_str(), 10);
}

void PluginComboList::selectionChangedEvent(AG_Event* event) {
	_selectionChangedListener->selectionChangedEvent();
}

void PluginComboList::expand() {
	AG_Expand(_combo);
}

void PluginComboList::expandHoriz() {
	AG_ExpandHoriz(_combo);
}

void PluginComboList::expandVert() {
	AG_ExpandVert(_combo);
}

} /* namespace JktPlugin */
