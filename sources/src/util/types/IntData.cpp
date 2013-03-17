/*
 * IntData.cpp
 *
 *  Created on: 17 mars 2013
 *      Author: Erwin
 */

#include "util/StreamUtils.h"
#include "util/types/IntData.h"

namespace JktUtils {

IntData::IntData(int value) {
	_value = value;
}

IntData::~IntData() {
}

void IntData::serialize(std::ostringstream& out) {
	StreamUtils::write(out, _value);
}

void IntData::unserialize(std::istringstream& in) {
	StreamUtils::read(in, _value);
}

int IntData::getValue() const {
	return _value;
}

}
