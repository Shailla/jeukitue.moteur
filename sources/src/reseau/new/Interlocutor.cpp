/*
 * Interlocutor.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */


#include <string>
#include <queue>

using namespace std;

#include "sdl.h"

#include "reseau/new/Interlocutor.h"

Interlocutor::Interlocutor(const string& name) {
	_name = name;
	_mutexDataReceived = SDL_CreateMutex();
	_mutexDataToSend = SDL_CreateMutex();
}

Interlocutor::~Interlocutor() {
}

const string& Interlocutor::getName() const {
	return _name;
}

void Interlocutor::addDataReceived(string* data) {
	SDL_LockMutex(_mutexDataReceived);

	if(data) {
		_dataReceived.push(data);
	}

	SDL_UnlockMutex(_mutexDataReceived);
}

string* Interlocutor::getDataReceived(void) {
	string* result;

	SDL_LockMutex(_mutexDataReceived);

	if(!_dataReceived.empty()) {
		result = _dataReceived.front();
		_dataReceived.pop();
	}
	else {
		result = NULL;
	}

	SDL_UnlockMutex(_mutexDataReceived);

	return result;
}

void Interlocutor::addDataToSend(string* data) {
	SDL_LockMutex(_mutexDataToSend);

	if(data) {
		_dataToSend.push(data);
	}

	SDL_UnlockMutex(_mutexDataToSend);
}

string* Interlocutor::getDataToSend(void) {
	string* result;

	SDL_LockMutex(_mutexDataToSend);

	if(!_dataToSend.empty()) {
		result = _dataToSend.front();
		_dataToSend.pop();
	}
	else {
		result = NULL;
	}

	SDL_UnlockMutex(_mutexDataToSend);

	return result;
}
