/*
 * Changement.cpp
 *
 *  Created on: 11 mars 2013
 *      Author: vgdj7997
 */

#include <string>

using namespace std;

#include "data/communication/message/Changement.h"

Changement::Changement(const string& dataType) {
	_dataType = dataType;
}

Changement::~Changement() {
}

string Changement::toString() {
	ostringstream str;
	serialize(str);

	ostringstream message;
	message << "{" << _dataType << "} : ";
	string::iterator iter;

	for(iter = str.str().begin() ; iter != str.str().end() ; iter++) {
		message << (int)*iter << " ";
	}

	return message.str();
}
