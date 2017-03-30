/*
 * StringData.cpp
 *
 *  Created on: 17 mars 2013
 *      Author: Erwin
 */

#include <string>

#include "util/StreamUtils.h"

#include "util/types/StringData.h"

using namespace std;

namespace jkt {

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

void StringData::serialize(std::ostringstream& out) const {
	StreamUtils::write(out, STRING_DATA_TYPE);
	StreamUtils::write(out, _value);
}

void StringData::toString(std::ostringstream& out) const {
	out << "string-'" << _value << "'";
}

const std::string& StringData::getValue() const {
	return _value;
}

Data& StringData::operator=(const Data& data) {
	_value = ((const StringData&)data)._value;
	return *this;
}

}
