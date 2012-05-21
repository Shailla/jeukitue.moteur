/*
 * PluginNumeric.cpp
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#include "plugin/api/gui/PluginNumeric.h"

using namespace std;

namespace JktPlugin {

PluginNumeric::PluginNumeric(AG_Widget* parent, const string& numericText, const string& numericUnite) {
	_numeric = AG_NumericalNewS(parent, AG_NUMERICAL_HFILL, numericText.c_str(), numericUnite.c_str());
	AG_BindFloat(_numeric, "value", &_value);
}

PluginNumeric::~PluginNumeric() {
}

float PluginNumeric::getValue() {
	return _value;
}

} /* namespace JktPlugin */
