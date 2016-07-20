/*
 * PluginNumeric.h
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#ifndef PLUGINNUMERIC_H_
#define PLUGINNUMERIC_H_

#include <string>

#include <agar/core.h>
#include <agar/gui.h>

#include "plugin/PluginContext.h"

namespace JktPlugin {

class PluginNumeric {
	friend class PluginRadioButtonProxy;

	AG_Numerical* _numeric;
	float _value;
public:
	PluginNumeric(AG_Widget* parent, const std::string& numericText, const std::string& numericUnite);
	virtual ~PluginNumeric();

	void expand();
	void expandHoriz();
	void expandVert();

	float getValue();
};

} /* namespace JktPlugin */
#endif /* PLUGINNUMERIC_H_ */
