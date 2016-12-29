/*
 * JsonString.cpp
 *
 *  Created on: 23 févr. 2016
 *      Author: VGDJ7997
 */

#include "reseau/web/json/JsonString.h"

namespace jkt {

JsonString::JsonString(const string& value) {
	_value = value;
}

JsonString::~JsonString() {
}

const string& JsonString::getValue() const {
	return _value;
}

const JsonNumber* JsonString::isJsonNumber() const {
	return 0;
}
const JsonString* JsonString::isJsonString() const {
	return this;
}

const JsonBoolean* JsonString::isJsonBoolean() const {
	return 0;
}

void JsonString::toJson(stringstream& buffer) {
	buffer << "\"" << _value << "\"";
}

} /* namespace jkt */
