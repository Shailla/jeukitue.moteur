/*
 * Interlocutor.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */


#include <string>
#include <queue>
#include <iostream>

#include "sdl.h"

#include "util/types/Bytes.h"

#include "reseau/new/NotConnectedInterlocutor2.h"

using namespace std;

NotConnectedInterlocutor2::NotConnectedInterlocutor2(SDL_cond* condDataToSend, SDL_mutex* mutexDataToSend) {
	_name = "";
	_mutexDataReceived = SDL_CreateMutex();
	_mutexDataToSend = mutexDataToSend;
	_condDataToSend = condDataToSend;
	_mutexNewInterlocutors = SDL_CreateMutex();
	_condIntelligence = NULL;
}

NotConnectedInterlocutor2::~NotConnectedInterlocutor2() {
}

void NotConnectedInterlocutor2::setCondIntelligence(SDL_cond* condIntelligence) {
	_condIntelligence = condIntelligence;
}

void NotConnectedInterlocutor2::setName(const string& name) {
	_name = name;
}

const string& NotConnectedInterlocutor2::getName() const {
	return _name;
}

void NotConnectedInterlocutor2::pushTechnicalMessageReceived(const IPaddress& address, jkt::Bytes* bytes) {
	SDL_LockMutex(_mutexDataReceived);

	if(bytes) {
		DataAddress* msg = new DataAddress(address, bytes);
		_technicalMessagesReceived.push(msg);
	}

	SDL_UnlockMutex(_mutexDataReceived);
	SDL_CondSignal(_condIntelligence);
}

void NotConnectedInterlocutor2::pushDataReceived(const IPaddress& address, jkt::Bytes* bytes) {
	SDL_LockMutex(_mutexDataReceived);

	if(bytes) {
		DataAddress* msg = new DataAddress(address, bytes);
		_dataReceived.push(msg);
	}

	SDL_UnlockMutex(_mutexDataReceived);
}

DataAddress* NotConnectedInterlocutor2::popTechnicalMessageReceived(void) {
	DataAddress* result;

	SDL_LockMutex(_mutexDataReceived);

	if(!_technicalMessagesReceived.empty()) {
		result = _technicalMessagesReceived.front();
		_technicalMessagesReceived.pop();
	}
	else {
		result = NULL;
	}

	SDL_UnlockMutex(_mutexDataReceived);

	return result;
}

DataAddress* NotConnectedInterlocutor2::popDataReceived(void) {
	DataAddress* result;

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

void NotConnectedInterlocutor2::waitDataToSend(int timeout) {
	SDL_LockMutex(_mutexDataToSend);
	SDL_CondWaitTimeout(_condDataToSend, _mutexDataToSend, timeout);
	SDL_UnlockMutex(_mutexDataToSend);
}

void NotConnectedInterlocutor2::pushTechnicalMessageToSend(jkt::Bytes* bytes) {
	SDL_LockMutex(_mutexDataToSend);

	if(bytes) {
		DataAddress* msg = new DataAddress(bytes);
		_technicalMessagesToSend.push(msg);
	}

	SDL_UnlockMutex(_mutexDataToSend);
	SDL_CondSignal(_condDataToSend);	// If we are waiting data to send, stop waiting
}


void NotConnectedInterlocutor2::pushTechnicalMessageToSend(const IPaddress& address, jkt::Bytes* bytes) {
	SDL_LockMutex(_mutexDataToSend);

	if(bytes) {
		DataAddress* msg = new DataAddress(address, bytes);
		_technicalMessagesToSend.push(msg);
	}

	SDL_UnlockMutex(_mutexDataToSend);
	SDL_CondSignal(_condDataToSend);	// If we are waiting data to send, stop waiting
}

void NotConnectedInterlocutor2::pushDataToSend(const IPaddress& address, jkt::Bytes* bytes) {
	SDL_LockMutex(_mutexDataToSend);

	if(bytes) {
		DataAddress* msg = new DataAddress(address, bytes);
		_dataToSend.push(msg);
	}

	SDL_UnlockMutex(_mutexDataToSend);
	SDL_CondSignal(_condDataToSend);	// If we are waiting data to send, stop waiting
}

DataAddress* NotConnectedInterlocutor2::popTechnicalMessageToSend(void) {
	DataAddress* result;

	SDL_LockMutex(_mutexDataToSend);

	if(!_technicalMessagesToSend.empty()) {
		result = _technicalMessagesToSend.front();
		_technicalMessagesToSend.pop();
	}
	else {
		result = NULL;
	}

	SDL_UnlockMutex(_mutexDataToSend);

	return result;
}

DataAddress* NotConnectedInterlocutor2::popDataToSend(void) {
	DataAddress* result;

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

Interlocutor2* NotConnectedInterlocutor2::popNewInterlocutor() {
	Interlocutor2* newInterlocutor;

	SDL_LockMutex(_mutexNewInterlocutors);

	if(!_newInterlocutors.empty()) {
		newInterlocutor = _newInterlocutors.front();
		_newInterlocutors.pop();
	}
	else {
		newInterlocutor = 0;
	}

	SDL_UnlockMutex(_mutexNewInterlocutors);

	return newInterlocutor;
}

void NotConnectedInterlocutor2::pushNewInterlocutor(Interlocutor2* newInterlocutor) {
	SDL_LockMutex(_mutexNewInterlocutors);
	_newInterlocutors.push(newInterlocutor);
	SDL_UnlockMutex(_mutexNewInterlocutors);
}
