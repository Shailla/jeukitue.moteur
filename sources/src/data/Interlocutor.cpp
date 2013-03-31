/*
 * Interlocutor.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */


#include <string>
#include <queue>

using namespace std;

#include "data/Interlocutor.h"

Interlocutor::Interlocutor(const string& name) {
	_name = name;
}

Interlocutor::~Interlocutor() {
}

const string& Interlocutor::getName() const {
	return _name;
}

void Interlocutor::addDataReceived(string* data) {
	if(data) {
		_dataReceived.push(data);
	}
}

string* Interlocutor::getDataReceived(void) {
	string* result;

	if(!_dataReceived.empty()) {
		result = _dataReceived.front();
		_dataReceived.pop();
	}
	else {
		result = NULL;
	}

	return result;
}

void Interlocutor::addDataToSend(string* data) {
	if(data) {
		_dataToSend.push(data);
	}
}

string* Interlocutor::getDataToSend(void) {
	string* result;

	if(!_dataToSend.empty()) {
		result = _dataToSend.front();
		_dataToSend.pop();
	}
	else {
		result = NULL;
	}

	return result;
}
