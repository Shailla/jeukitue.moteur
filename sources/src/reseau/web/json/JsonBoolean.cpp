/*
 * JsonBoolean.cpp
 *
 *  Created on: 23 févr. 2016
 *      Author: VGDJ7997
 */

#include "reseau/web/json/JsonBoolean.h"

namespace jkt {

JsonBoolean::JsonBoolean(bool value) {
	_value = value;
}

JsonBoolean::~JsonBoolean() {
}

bool JsonBoolean::getValue() const {
	return _value;
}

const JsonNumber* JsonBoolean::isJsonNumber() const {
	return 0;
}

const JsonString* JsonBoolean::isJsonString() const {
	return 0;
}

const JsonBoolean* JsonBoolean::isJsonBoolean() const {
	return this;
}

void JsonBoolean::toJson(stringstream& buffer) {
	buffer << (_value?"true":"false");
}

} /* namespace jkt */
