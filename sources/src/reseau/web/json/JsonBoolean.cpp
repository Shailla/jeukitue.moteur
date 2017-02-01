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

JsonNumber* JsonBoolean::isJsonNumber() {
	return 0;
}

JsonString* JsonBoolean::isJsonString() {
	return 0;
}

JsonBoolean* JsonBoolean::isJsonBoolean() {
	return this;
}

JsonObject* JsonBoolean::isJsonObject() {
	return 0;
}

JsonList* JsonBoolean::isJsonList() {
	return 0;
}

void JsonBoolean::toJson(stringstream& buffer) {
	buffer << (_value?"true":"false");
}

} /* namespace jkt */
