/*
 * Changement.cpp
 *
 *  Created on: 11 mars 2013
 *      Author: vgdj7997
 */

#include <string>
#include <iostream>

using namespace std;

#include "data/communication/message/Changement.h"

Changement::Changement(const string& dataType, int majorPriority, int minorPriority) {
	_dataType = dataType;
	_majorPriority = majorPriority;
	_minorPriority = minorPriority;
}

Changement::~Changement() {
}

int Changement::getMajorPriority() const {
	return _majorPriority;
}

int Changement::getMinorPriority() const {
	return _minorPriority;
}

bool Changement::highestPriority(const Changement* left, const Changement* right) {
	if(left->getRootDistance() != right->getRootDistance()) {
		return left->getRootDistance() < right->getRootDistance();
	}
	else if(left->_majorPriority != right->_majorPriority) {
		return left->_majorPriority > right->_majorPriority;
	}
	else if(left->_minorPriority != right->_minorPriority) {
		return left->_minorPriority > right->_minorPriority;
	}
	else {
		return left > right;
	}
}

string Changement::toString() const {
	ostringstream sstr;
	serialize(sstr);
	string str = sstr.str();

	ostringstream message;

	message << "{" << _dataType << " - ";

	string::iterator iter;

	for(iter = str.begin() ; iter != str.end() ; iter++) {
		message << (int)*iter << " ";
	}

	message << "}";

	return message.str();
}
