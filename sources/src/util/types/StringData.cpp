/*
 * StringData.cpp
 *
 *  Created on: 17 mars 2013
 *      Author: Erwin
 */

#include <string>

using namespace std;

#include "util/StreamUtils.h"

#include "util/types/StringData.h"

namespace JktUtils {

StringData::StringData(const char* value) {
	_value = value;
}

StringData::StringData(const string& value) {
	_value = value;
}

StringData::StringData(std::istringstream& in) {
	StreamUtils::read(in, _value);
}

StringData::~StringData() {
}

void StringData::serialize(std::ostringstream& out) {
	StreamUtils::write(out, STRING_DATA_TYPE);
	StreamUtils::write(out, _value);
}

void StringData::toString(std::ostringstream& out) const {
	out << "string-'" << _value << "'";
}

const std::string& StringData::getValue() const {
	return _value;
}

}
