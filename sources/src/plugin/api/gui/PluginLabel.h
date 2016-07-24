/*
 * PluginLabel.h
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#ifndef PLUGINLABEL_H_
#define PLUGINLABEL_H_

#include <string>

#include <agar/core.h>
#include <agar/gui.h>

#include "plugin/PluginContext.h"

namespace jkt {

class PluginLabel {
	AG_Label* _label;

public:
	PluginLabel(AG_Widget* parent, const std::string& labelText);
	virtual ~PluginLabel();

	void expand();
	void expandHoriz();
	void expandVert();
};

} /* namespace jkt */
#endif /* PLUGINLABEL_H_ */
