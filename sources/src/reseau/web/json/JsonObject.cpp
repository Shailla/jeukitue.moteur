/*
 * JsonString.cpp
 *
 *  Created on: 23 févr. 2016
 *      Author: VGDJ7997
 */

#include "reseau/web/json/JsonObject.h"

#include "reseau/web/json/JsonPair.h"
#include "reseau/web/json/JsonString.h"
#include "reseau/web/json/JsonList.h"


namespace jkt {

JsonObject::JsonObject() {
}

JsonObject::~JsonObject() {
	for(JsonPair* pair : _pairs) {
		delete pair;
	}
}

void JsonObject::addString(const string& name, const string& value) {
	JsonPair* pair = new JsonPair(name, (JsonValue*)new JsonString(value));
	_pairs.push_back(pair);
}

JsonObject& JsonObject::addObject(const string& name) {
	JsonObject* var = new JsonObject();
	JsonPair* pair = new JsonPair(name, var);
	_pairs.push_back(pair);
	return *var;
}

JsonList& JsonObject::addList(const string& name) {
	JsonList* var = new JsonList();
	JsonPair* pair = new JsonPair(name, var);
	_pairs.push_back(pair);
	return *var;
}

void JsonObject::toJson(stringstream& buffer) {
	buffer << "{";

	bool first = true;

	for(JsonPair* pair : _pairs) {
		if(!first) {
			buffer << ",";
		}

		first = false;

		pair->toJson(buffer);
	}

	buffer << "}";
}

} /* namespace jkt */
