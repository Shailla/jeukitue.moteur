/*
 * LuaWindow.cpp
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#include "plugin/api/gui/LuaWindow.h"

namespace JktPlugin {

LuaWindow::LuaWindow() {
	_window = AG_WindowNew(AG_WINDOW_NOBUTTONS | AG_WINDOW_NOMOVE);
}

LuaWindow::~LuaWindow() {
}

void LuaWindow::setSize(const int x, const int y) {
	AG_WindowSetGeometryAlignedPct(_window, AG_WINDOW_MC, x, y);
}

void LuaWindow::setTitle(const char titre[]) {
	AG_WindowSetCaption(_window, titre);
}

void LuaWindow::show() {
	AG_WindowShow(_window);
}

void LuaWindow::hide() {
	AG_WindowHide(_window);
}

} /* namespace JktPlugin */
