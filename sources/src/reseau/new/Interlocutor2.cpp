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

#include "util/types/Bytes.h"

#include "reseau/new/Interlocutor2.h"

Interlocutor2::Interlocutor2(SDL_cond* condDataToSend, SDL_mutex* mutexDataToSend) {
	_name = "";
	_mutexDataReceived = SDL_CreateMutex();
	_mutexDataToSend = mutexDataToSend;
	_condDataToSend = condDataToSend;
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

void Interlocutor2::pushTechnicalMessageReceived(JktUtils::Bytes* bytes) {
	SDL_LockMutex(_mutexDataReceived);

	if(bytes) {
		_technicalMessagesReceived.push(bytes);
	}

	SDL_UnlockMutex(_mutexDataReceived);
	SDL_CondSignal(_condIntelligence);
}

void Interlocutor2::pushDataReceived(JktUtils::Bytes* bytes) {
	SDL_LockMutex(_mutexDataReceived);

	if(bytes) {
		_dataReceived.push(bytes);
	}

	SDL_UnlockMutex(_mutexDataReceived);
}

JktUtils::Bytes* Interlocutor2::popTechnicalMessageReceived(void) {
	JktUtils::Bytes* result;

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

JktUtils::Bytes* Interlocutor2::popDataReceived(void) {
	JktUtils::Bytes* result;

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
	SDL_UnlockMutex(_mutexDataToSend);
}

void Interlocutor2::pushTechnicalMessageToSend(JktUtils::Bytes* bytes) {
	SDL_LockMutex(_mutexDataToSend);

	if(bytes) {
		_technicalMessagesToSend.push(bytes);
	}

	SDL_UnlockMutex(_mutexDataToSend);
	SDL_CondSignal(_condDataToSend);	// If we are waiting data to send, stop waiting
}

void Interlocutor2::pushDataToSend(JktUtils::Bytes* bytes) {
	SDL_LockMutex(_mutexDataToSend);

	if(bytes) {
		_dataToSend.push(bytes);
	}

	SDL_UnlockMutex(_mutexDataToSend);
	SDL_CondSignal(_condDataToSend);	// If we are waiting data to send, stop waiting
}

JktUtils::Bytes* Interlocutor2::popTechnicalMessageToSend(void) {
	JktUtils::Bytes* result;

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

JktUtils::Bytes* Interlocutor2::popDataToSend(void) {
	JktUtils::Bytes* result;

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

int Interlocutor2::countTechnicalMessagesReceived(void) {
	SDL_LockMutex(_mutexDataReceived);

	int result = _technicalMessagesReceived.size();

	SDL_UnlockMutex(_mutexDataReceived);

	return result;
}

int Interlocutor2::countDataReceived(void) {
	SDL_LockMutex(_mutexDataReceived);

	int result = _dataReceived.size();

	SDL_UnlockMutex(_mutexDataReceived);

	return result;
}

int Interlocutor2::countTechnicalMessagesToSend(void) {
	SDL_LockMutex(_mutexDataToSend);

	int result = _technicalMessagesToSend.size();

	SDL_UnlockMutex(_mutexDataToSend);

	return result;
}

int Interlocutor2::countDataToSend(void) {
	SDL_LockMutex(_mutexDataToSend);

	int result = _dataToSend.size();

	SDL_UnlockMutex(_mutexDataToSend);

	return result;
}
