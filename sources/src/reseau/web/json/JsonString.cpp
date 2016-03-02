/*
 * JsonString.cpp
 *
 *  Created on: 23 févr. 2016
 *      Author: VGDJ7997
 */

#include "reseau/web/json/JsonString.h"

namespace JktNet {

JsonString::JsonString(const string& value) {
	_value = value;
}

JsonString::~JsonString() {
}

void JsonString::toJson(stringstream& buffer) {
	buffer << "\"" << _value << "\"";
}

} /* namespace JktNet */
