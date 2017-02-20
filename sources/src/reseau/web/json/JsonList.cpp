/*
 * JsonList.cpp
 *
 *  Created on: 23 févr. 2016
 *      Author: VGDJ7997
 */

#include "reseau/web/json/JsonList.h"

#include "reseau/web/json/JsonPair.h"
#include "reseau/web/json/JsonString.h"
#include "reseau/web/json/JsonNumber.h"
#include "reseau/web/json/JsonBoolean.h"
#include "reseau/web/json/JsonObject.h"

namespace jkt {

JsonList::JsonList() {
}

JsonList::~JsonList() {
	for(JsonValue* value : _values) {
		delete value;
	}
}

void JsonList::toJson(std::stringstream& buffer) {
	buffer << "[";

	bool first = true;

	for(JsonValue* value : _values) {
		if(!first) {
			buffer << ",";
		}

		first = false;

		value->toJson(buffer);
	}

	buffer << "]";
}

void JsonList::addValue(JsonValue* value) {
	_values.push_back(value);
}

JsonString& JsonList::addString(const string& value) {
	JsonString* var = new JsonString(value);
	_values.push_back(var);
	return *var;
}

JsonNumber& JsonList::addNumber(unsigned int value) {
	JsonNumber* var = new JsonNumber(value);
	_values.push_back(var);
	return *var;
}

JsonNumber& JsonList::addNumber(int value) {
	JsonNumber* var = new JsonNumber(value);
	_values.push_back(var);
	return *var;
}

JsonNumber& JsonList::addNumber(long value) {
	JsonNumber* var = new JsonNumber(value);
	_values.push_back(var);
	return *var;
}

JsonBoolean& JsonList::addBoolean(bool value) {
	JsonBoolean* var = new JsonBoolean(value);
	_values.push_back(var);
	return *var;
}

JsonObject& JsonList::addObject() {
	JsonObject* var = new JsonObject();
	_values.push_back(var);
	return *var;
}

JsonList& JsonList::addList() {
	JsonList* var = new JsonList();
	_values.push_back(var);
	return *var;
}

const vector<JsonValue*>& JsonList::getValue() {
	return _values;
}

JsonNumber* JsonList::isJsonNumber() {
	return 0;
}
JsonString* JsonList::isJsonString() {
	return 0;
}

JsonBoolean* JsonList::isJsonBoolean() {
	return 0;
}

JsonObject* JsonList::isJsonObject() {
	return 0;
}

JsonList* JsonList::isJsonList() {
	return this;
}

} /* namespace jkt */
