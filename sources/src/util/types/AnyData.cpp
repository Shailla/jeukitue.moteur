/*
 * Data.cpp
 *
 *  Created on: 17 mars 2013
 *      Author: Erwin
 */

#include "util/Trace.h"
#include "util/types/AnyData.h"
#include "util/StreamUtils.h"
#include "util/exception/UnserializeException.h"

namespace jkt {

AnyData::AnyData() {
	_type = UNKNOWN_DATA_TYPE;
	_int_value = 0;
	_float_value = 0.0f;
	_string_value = "";
}

AnyData::AnyData(const int& value) {
	_type = INT_DATA_TYPE;
	_int_value = value;
	_float_value = 0.0f;
	_string_value = "";
}

AnyData::AnyData(const float& value) {
	_type = FLOAT_DATA_TYPE;
	_int_value = 0;
	_float_value = value;
	_string_value = "";
}

AnyData::AnyData(const string& value) {
	_type = STRING_DATA_TYPE;
	_int_value = 0;
	_float_value = 0.0f;
	_string_value = value;
}

AnyData::AnyData(std::istringstream& in) {
	unserialize(*this, in);
}

AnyData::~AnyData() {
}

void AnyData::unserialize(AnyData& data, std::istringstream& in) throw(UnserializeException) {
	int type;

	StreamUtils::read(in, type);

	switch(type) {
	case INT_DATA_TYPE:
		data._type = INT_DATA_TYPE;
		StreamUtils::read(in, data._int_value);
		break;

	case FLOAT_DATA_TYPE:
		data._type = FLOAT_DATA_TYPE;
		StreamUtils::read(in, data._float_value);
		break;

	case STRING_DATA_TYPE:
		data._type = STRING_DATA_TYPE;
		StreamUtils::read(in, data._string_value);
		break;

	default:
		throw UnserializeException();
		break;
	}
}

void AnyData::toString(std::ostringstream& out) const {
	switch(_type) {
	case INT_DATA_TYPE:
		out << "int-'" << _int_value << "'";
		break;

	case FLOAT_DATA_TYPE:
		out << "float-'" << _float_value << "'";
		break;

	case STRING_DATA_TYPE:
		out << "string-'" << _string_value << "'";
		break;

	default:
		out << "unknown-xxx";
		LOGERROR(("Type de AnyData inconnu"));
		break;
	}
}

std::string AnyData::toString() const {
	std::ostringstream ss;
	this->toString(ss);

	return ss.str();
}

void AnyData::serialize(std::ostringstream& out) const {
	switch(_type) {
	case INT_DATA_TYPE:
		StreamUtils::write(out, INT_DATA_TYPE);
		StreamUtils::write(out, _int_value);
		break;

	case FLOAT_DATA_TYPE:
		StreamUtils::write(out, FLOAT_DATA_TYPE);
		StreamUtils::write(out, _float_value);
		break;

	case STRING_DATA_TYPE:
		StreamUtils::write(out, STRING_DATA_TYPE);
		StreamUtils::write(out, _string_value);
		break;

	default:
		LOGERROR(("Type de AnyData inconnu"));
		break;
	}
}

AnyData& AnyData::operator=(const AnyData& data) {
	_type = data._type;

	switch(_type) {
	case INT_DATA_TYPE:
		_int_value = data._int_value;
		break;

	case FLOAT_DATA_TYPE:
		_float_value = data._float_value;
		break;

	case STRING_DATA_TYPE:
		_string_value = data._string_value;
		break;

	default:
		LOGERROR(("Type de AnyData inconnu"));
		break;
	}

	return *this;
}

void AnyData::update(const int& value) {
	_type = INT_DATA_TYPE;
	_int_value = value;
	_float_value = 0.0f;
	_string_value = "";
}

void AnyData::update(const float& value) {
	_type = FLOAT_DATA_TYPE;
	_int_value = 0;
	_float_value = value;
	_string_value = "";
}

void AnyData::update(const string& value) {
	_type = STRING_DATA_TYPE;
	_int_value = 0;
	_float_value = 0.0f;
	_string_value = value;
}

bool AnyData::isInt() const {
	return _type == INT_DATA_TYPE;
}

bool AnyData::isFloat() const {
	return _type == FLOAT_DATA_TYPE;
}

bool AnyData::isString() const {
	return _type == STRING_DATA_TYPE;
}

int AnyData::getValueInt() const {
	return _int_value;
}

float AnyData::getValueFloat() const {
	return _float_value;
}

string AnyData::getValueString() const {
	return _string_value;
}

};
