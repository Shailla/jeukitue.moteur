/*
 * PluginWindow.h
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#ifndef PLUGINWINDOW_H_
#define PLUGINWINDOW_H_

#include <agar/core.h>
#include <agar/gui.h>

#include "plugin/api/gui/PluginPanel.h"
#include "plugin/PluginContext.h"

namespace JktPlugin {

class PluginWindow : public PluginPanel {
	friend class PluginWindowProxy;

	AG_Window* _window;
public:
	PluginWindow(PluginContext* pluginContext);
	virtual ~PluginWindow();

	AG_Widget* getParent();

protected:
	// Visibilité
	void show();
	void hide();

	// Apparence
	void setTitle(const char titre[]);
	void setPosition(const int x, const int y);
	void setSize(const int w, const int h);
};

} /* namespace JktPlugin */
#endif /* PLUGINWINDOW_H_ */
