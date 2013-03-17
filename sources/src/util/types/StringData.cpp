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

StringData::StringData(const string& value) {
	_value = value;
}

StringData::~StringData() {
}

void StringData::serialize(std::ostringstream& out) {
	StreamUtils::write(out, _value);
}

void StringData::unserialize(std::istringstream& in) {
	StreamUtils::read(in, _value);
}

}
