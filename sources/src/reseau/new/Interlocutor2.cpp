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

#include "reseau/new/Interlocutor2.h"

Interlocutor2::Interlocutor2() {
	_name = "";
	_mutexDataReceived = SDL_CreateMutex();
	_mutexDataToSend = SDL_CreateMutex();
	_condDataToSend = SDL_CreateCond();
	_condIntelligence = NULL;
}

Interlocutor2::~Interlocutor2() {
}

void Interlocutor2::setCondIntelligence(SDL_cond* condIntelligence) {
	_condIntelligence = condIntelligence;
}

void Interlocutor2::setName(const string& name) {
	_name = name;
}

const string& Interlocutor2::getName() const {
	return _name;
}

void Interlocutor2::pushTechnicalMessageReceived(const IPaddress& address, char* data) {
	SDL_LockMutex(_mutexDataReceived);

	if(data) {
		DataAddress* msg = new DataAddress(address, data);
		_technicalMessagesReceived.push(msg);
	}

	SDL_UnlockMutex(_mutexDataReceived);
	SDL_CondSignal(_condIntelligence);
}

void Interlocutor2::pushDataReceived(char* data) {
	SDL_LockMutex(_mutexDataReceived);

	if(data) {
		_dataReceived.push(data);
	}

	SDL_UnlockMutex(_mutexDataReceived);
}

Interlocutor2::DataAddress* Interlocutor2::popTechnicalMessageReceived(void) {
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

char* Interlocutor2::popDataReceived(void) {
	char* result;

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

void Interlocutor2::waitDataToSend(int timeout) {
	SDL_LockMutex(_mutexDataToSend);
	SDL_CondWaitTimeout(_condDataToSend, _mutexDataToSend, timeout);
}

void Interlocutor2::pushTechnicalMessageToSend(char* data) {
	SDL_LockMutex(_mutexDataToSend);

	if(data) {
		_technicalMessagesToSend.push(data);
	}

	SDL_UnlockMutex(_mutexDataToSend);
	SDL_CondSignal(_condDataToSend);	// If we are waiting data to send, stop waiting
}

void Interlocutor2::pushDataToSend(char* data) {
	SDL_LockMutex(_mutexDataToSend);

	if(data) {
		_dataToSend.push(data);
	}

	SDL_UnlockMutex(_mutexDataToSend);
	SDL_CondSignal(_condDataToSend);	// If we are waiting data to send, stop waiting
}

char* Interlocutor2::popTechnicalMessageToSend(void) {
	char* result;

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

char* Interlocutor2::popDataToSend(void) {
	char* result;

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
