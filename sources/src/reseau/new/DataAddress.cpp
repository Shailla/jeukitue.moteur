/*
 * DataAddress.cpp
 *
 *  Created on: 25 juin 2013
 *      Author: Erwin
 */

#include "reseau/new/DataAddress.h"

DataAddress::DataAddress(char* data, int size) {
	_bytes = new JktUtils::Bytes(data, size);
}

DataAddress::DataAddress(JktUtils::Bytes* bytes) {
	_bytes = bytes;
}

DataAddress::DataAddress(const IPaddress& address, char* data, int size) {
	_address = address;
	_bytes = new JktUtils::Bytes(data, size);
}

DataAddress::DataAddress(const IPaddress& address, JktUtils::Bytes* bytes) {
	_address = address;
	_bytes = bytes;
}

DataAddress::~DataAddress() {
	delete _bytes;
}

const IPaddress& DataAddress::getAddress() const {
	return _address;
}

JktUtils::Bytes* DataAddress::getBytes() {
	return _bytes;
}
