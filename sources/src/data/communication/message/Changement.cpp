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

Changement::Changement(const string& dataType, int priority) {
	_dataType = dataType;
	_priority = priority;
}

Changement::~Changement() {
}

int Changement::getPriority() const {
	return _priority;
}

bool Changement::highestPriority(const Changement* left, const Changement* right) {
	int leftPriority = left->getPriority();
	int rightPriority = right->getPriority();

	cout << endl << "LEFT : '" << left->toString() << "' RIGHT : '" << right->toString() << "'" << flush;

	if(leftPriority > rightPriority) {
		return true;
	}
	else if(leftPriority < rightPriority) {
		return false;
	}
	else {
		return left->getRootDistance() < right->getRootDistance();
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
