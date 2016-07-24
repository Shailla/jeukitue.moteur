/*
 * Bytes.cpp
 *
 *  Created on: 25 juin 2013
 *      Author: vgdj7997
 */

#include "util/types/Bytes.h"

namespace jkt {

using namespace std;

Bytes::Bytes(char* bytes, int size) {
	_bytes = bytes;
	_size = size;
}

Bytes::Bytes(const string& str) {
	_size = str.size();

	_bytes = new char[_size];
	str.copy(_bytes, _size);
}

Bytes::~Bytes() {
	delete[] _bytes;
}

const char* Bytes::getBytes() const {
	return _bytes;
}

int Bytes::size() {
	return _size;
}

} /* namespace jkt */
