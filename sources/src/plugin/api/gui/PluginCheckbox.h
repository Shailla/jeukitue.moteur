/*
 * PluginCheckbox.h
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#ifndef PLUGINCHECKBOX_H_
#define PLUGINCHECKBOX_H_

#include <string>

#include <agar/core.h>
#include <agar/gui.h>

#include "plugin/PluginContext.h"

namespace jkt {

class PluginCheckbox {
	friend class PluginRadioButtonProxy;

	AG_Checkbox* _checkbox;
	int _value;
public:
	PluginCheckbox(AG_Widget* parent, const std::string& checkboxText);
	virtual ~PluginCheckbox();

	bool getValue();
	void setValue(const bool value);
};

} /* namespace jkt */
#endif /* PLUGINCHECKBOX_H_ */
