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
	_numeric = AG_NumericalNewS(parent, AG_NUMERICAL_HFILL, numericUnite.c_str(), numericText.c_str());
	AG_BindFloat(_numeric, "value", &_value);
}

PluginNumeric::~PluginNumeric() {
}

float PluginNumeric::getValue() {
	return _value;
}

void PluginNumeric::expand() {
	AG_Expand(_numeric);
}

void PluginNumeric::expandHoriz() {
	AG_ExpandHoriz(_numeric);
}

void PluginNumeric::expandVert() {
	AG_ExpandVert(_numeric);
}

} /* namespace JktPlugin */
