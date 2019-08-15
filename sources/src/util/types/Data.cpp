/*
 * Data.cpp
 *
 *  Created on: 17 mars 2013
 *      Author: Erwin
 */

#include "util/types/Data.h"
#include "util/types/IntData.h"
#include "util/types/FloatData.h"
#include "util/types/StringData.h"
#include "util/StreamUtils.h"
#include "util/exception/UnserializeException.h"

namespace jkt {

Data::Data() {
}

Data::~Data() {
}

Data* Data::unserialize(std::istringstream& in) {
	int type;

	StreamUtils::read(in, type);

	Data* data;

	switch(type) {
	case INT_DATA_TYPE:
		data = new IntData(in);
		break;

	case FLOAT_DATA_TYPE:
		data = new FloatData(in);
		break;

	case STRING_DATA_TYPE:
		data = new StringData(in);
		break;

	default:
		throw UnserializeException();
		break;
	}

	return data;
}

std::string Data::toString() const {
	std::ostringstream ss;
	this->toString(ss);

	return ss.str();
}

};
