/*
 * JsonUtils.cpp
 *
 *  Created on: 23 févr. 2016
 *      Author: VGDJ7997
 */

#include <sstream>
#include <string>

#include "reseau/web/json/JsonValue.h"

using namespace std;

namespace JktNet {

JsonValue::JsonValue() {
}

JsonValue::~JsonValue() {
}

string JsonValue::toString() {
	stringstream str;
	toJson(str);
	return str.str();
}

} /* namespace JktNet */
