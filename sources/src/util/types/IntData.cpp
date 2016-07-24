/*
 * IntData.cpp
 *
 *  Created on: 17 mars 2013
 *      Author: Erwin
 */

#include "util/StreamUtils.h"
#include "util/types/IntData.h"

namespace jkt {

IntData::IntData(int value) {
	_value = value;
}

IntData::IntData(std::istringstream& in) {
	StreamUtils::read(in, _value);
}

IntData::~IntData() {
}

void IntData::serialize(std::ostringstream& out) const {
	StreamUtils::write(out, INT_DATA_TYPE);
	StreamUtils::write(out, _value);
}

void IntData::toString(std::ostringstream& out) const {
	out << "int-'" << _value << "'";
}

int IntData::getValue() const {
	return _value;
}

Data& IntData::operator=(const Data& data) {
	_value = ((const IntData&)data)._value;
	return *this;
}

}
