/*
 * JsonNumber.cpp
 *
 *  Created on: 23 févr. 2016
 *      Author: VGDJ7997
 */

#include "reseau/web/json/JsonNumber.h"

namespace jkt {

JsonNumber::JsonNumber(const unsigned int& value) {
	_value = value;
}

JsonNumber::JsonNumber(const int& value) {
	_value = value;
}

JsonNumber::JsonNumber(const long& value) {
	_value = value;
}

JsonNumber::~JsonNumber() {
}

void JsonNumber::toJson(stringstream& buffer) {
	buffer << _value;
}

} /* namespace jkt */
