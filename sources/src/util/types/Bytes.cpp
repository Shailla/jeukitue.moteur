/*
 * Bytes.cpp
 *
 *  Created on: 25 juin 2013
 *      Author: vgdj7997
 */

#include "util/types/Bytes.h"

namespace JktUtils {

using namespace std;

Bytes::Bytes(char* bytes, int size) {
	_bytes = bytes;
	_size = size;
}

Bytes::Bytes(const string& str) {
	_bytes = new char[str.size()]();
	str.copy(_bytes, str.size());

	_size = str.size();
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

} /* namespace JktUtils */
