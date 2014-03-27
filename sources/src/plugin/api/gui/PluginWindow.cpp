/*
 * PluginWindow.cpp
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#include "plugin/api/gui/PluginWindow.h"

namespace JktPlugin {

PluginWindow::PluginWindow(PluginContext* pluginContext) {
	_window = AG_WindowNew(AG_WINDOW_NOBUTTONS | AG_WINDOW_NOMOVE);
}

PluginWindow::~PluginWindow() {
	if(_window) {
		AG_ObjectDelete(_window);
	}
}

AG_Widget* PluginWindow::getParent() {
	return (AG_Widget*)_window;
}

void PluginWindow::setPosition(const int x, const int y) {
	AG_WindowSetGeometry(_window, x, y, _window->r.w, _window->r.h);
}

void PluginWindow::setSize(const int w, const int h) {
	AG_WindowSetGeometry(_window, _window->r.x, _window->r.y, w, h);
}

void PluginWindow::setTitle(const char titre[]) {
	AG_WindowSetCaption(_window, titre);
}

void PluginWindow::show() {
	AG_WindowShow(_window);
	AG_WidgetFocus(_window);
}

void PluginWindow::hide() {
	AG_WindowHide(_window);
	AG_WidgetUnfocus(_window);
}

} /* namespace JktPlugin */
