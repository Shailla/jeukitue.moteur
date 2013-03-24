/*
 * IntData.cpp
 *
 *  Created on: 17 mars 2013
 *      Author: Erwin
 */

#include "util/StreamUtils.h"
#include "util/types/FloatData.h"

namespace JktUtils {

FloatData::FloatData(float value) {
	_value = value;
}

FloatData::FloatData(std::istringstream& in) {
	StreamUtils::read(in, _value);
}

FloatData::~FloatData() {
}

void FloatData::serialize(std::ostringstream& out) {
	StreamUtils::write(out, FLOAT_DATA_TYPE);
	StreamUtils::write(out, _value);
}

void FloatData::serializeHumanReadable(std::ostringstream& out) {
	out << "float-'" << _value << "'";
}

int FloatData::getValue() const {
	return _value;
}

}
