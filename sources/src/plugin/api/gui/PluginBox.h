/*
 * PluginBox.h
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#ifndef PLUGINBOX_H_
#define PLUGINBOX_H_

#include <string>

#include <agar/core.h>
#include <agar/gui.h>

#include "plugin/api/gui/PluginPanel.h"

namespace jkt {

class PluginBox : public PluginPanel{
	friend class PluginBoxProxy;

	AG_Box* _box;
public:
	PluginBox(AG_Widget* parent, ag_box_type checkboxType);
	virtual ~PluginBox();

	AG_Widget* getParent();
};

} /* namespace jkt */
#endif /* PLUGINBOX_H_ */
