/*
 * JsonString.cpp
 *
 *  Created on: 23 févr. 2016
 *      Author: VGDJ7997
 */

#include "reseau/web/json/JsonString.h"

using namespace std;

namespace jkt {

JsonString::JsonString(const string& value) {
	_value = value;
}

JsonString::~JsonString() {
}

const string& JsonString::getValue() const {
	return _value;
}

JsonNumber* JsonString::isJsonNumber() {
	return 0;
}
JsonString* JsonString::isJsonString() {
	return this;
}

JsonBoolean* JsonString::isJsonBoolean() {
	return 0;
}

JsonObject* JsonString::isJsonObject() {
	return 0;
}

JsonList* JsonString::isJsonList() {
	return 0;
}

void JsonString::toJson(stringstream& buffer) {
	buffer << "\"" << _value << "\"";
}

} /* namespace jkt */
