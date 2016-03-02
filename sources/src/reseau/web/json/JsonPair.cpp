/*
 * JsonPair.cpp
 *
 *  Created on: 24 févr. 2016
 *      Author: VGDJ7997
 */

#include "reseau/web/json/JsonPair.h"

using namespace std;

namespace JktNet {

JsonPair::JsonPair(const string& name, JsonValue* value) {
	_name = name;
	_value = value;
}

JsonPair::~JsonPair() {
	if(_value) {
		delete _value;
	}
}

void JsonPair::toJson(stringstream& buffer) {
	buffer << "\"" << _name << "\":";
	_value->toJson(buffer);
}

string JsonPair::toJson() {
	stringstream str;
	toJson(str);
	return str.str();
}

} /* namespace JktNet */
