/*
 * OMessageStream.cpp
 *
 *  Created on: 11 mars 2013
 *      Author: Erwin
 */

#include <vector>

using namespace std;

#include "data/communication/OMessageStream.h"

OMessageStream& OMessageStream::operator<<(std::vector<int>& data) {
	*this << data.size();

	vector<int>::iterator it;

	for(it = data.begin() ; it != data.end() ; it++) {
		*this << *it;
	}

	return *this;
}
