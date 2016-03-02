/*
 * JsonList.cpp
 *
 *  Created on: 23 févr. 2016
 *      Author: VGDJ7997
 */

#include "reseau/web/json/JsonList.h"

#include "reseau/web/json/JsonPair.h"
#include "reseau/web/json/JsonString.h"
#include "reseau/web/json/JsonObject.h"

namespace JktNet {

JsonList::JsonList() {
}

JsonList::~JsonList() {
	for(JsonValue* value : _values) {
		delete value;
	}
}

JsonString& JsonList::addString(const string& value) {
	JsonString* var = new JsonString(value);
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

void JsonList::toJson(stringstream& buffer) {
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

} /* namespace JktNet */
