/*
 * TechnicalInterlocutor.cpp
 *
 *  Created on: 17 juin 2013
 *      Author: vgdj7997
 */

#include <string>
#include <sstream>

using namespace std;

#include "SDL.h"

#include "util/Trace.h"
#include "reseau/new/message/TechnicalMessage.h"

#include "reseau/new/TechnicalInterlocutor.h"

TechnicalInterlocutor::TechnicalInterlocutor(const string& name, Uint16 portLocal) {
	_name = name;
	_localPort = portLocal;

	_mutexIntelligence = SDL_CreateMutex();
	_condIntelligence = SDL_CreateCond();

	setConnexionStatus(STOPPED);
}

TechnicalInterlocutor::~TechnicalInterlocutor() {
}

void TechnicalInterlocutor::close() {

}

void TechnicalInterlocutor::startProcesses() {
	_threadReceiving  = SDL_CreateThread(startReceivingProcess, (void*)this);
	_threadSending  = SDL_CreateThread(startSendingProcess, (void*)this);
	_threadIntelligence  = SDL_CreateThread(startIntelligenceProcess, (void*)this);
}

void TechnicalInterlocutor::waitProcessesFinished() {
	int status;

	SDL_WaitThread(_threadReceiving, &status);
	SDL_WaitThread(_threadSending, &status);
	SDL_WaitThread(_threadIntelligence, &status);
}

int TechnicalInterlocutor::startReceivingProcess(void* thiz) {
	((TechnicalInterlocutor*)thiz)->receivingProcess();
	return 0;
}

int TechnicalInterlocutor::startSendingProcess(void* thiz) {
	((TechnicalInterlocutor*)thiz)->sendingProcess();
	return 0;
}

int TechnicalInterlocutor::startIntelligenceProcess(void* thiz) {
	((TechnicalInterlocutor*)thiz)->intelligenceProcess();
	return 0;
}

void TechnicalInterlocutor::setConnexionStatus(CONNEXION_STATUS status) {
	_status = status;
	SDL_CondSignal(_condIntelligence);

	switch(_status) {
	case STOPPED:
		log("I am now STOPPED");
		break;
	case CONNECTING:
		log("I am now CONNECTING");
		break;
	case CONNECTED:
		log("I am now CONNECTED");
		break;
	case STOPPING:
		log("I am now STOPPING");
		break;
	default:
		stringstream message;
		message << "We should never be here " << __FILE__ << " " << __LINE__;
		log(message);
		break;
	}
}

TechnicalInterlocutor::CONNEXION_STATUS TechnicalInterlocutor::getConnexionStatus() const {
	return _status;
}

Uint16 TechnicalInterlocutor::getLocalPort() const {
	return _localPort;
}

SDL_mutex* TechnicalInterlocutor::getMutexIntelligence() {
	return _mutexIntelligence;
}

SDL_cond* TechnicalInterlocutor::getCondIntelligence() {
	return _condIntelligence;
}

void TechnicalInterlocutor::log(const char* message) {
	TRACE().info("'%s' : '%s'", _name.c_str(), message);
}

void TechnicalInterlocutor::log(const string& message) {
	TRACE().info("'%s' : '%s'", _name.c_str(), message.c_str());
}

void TechnicalInterlocutor::log(const stringstream& message) {
	TRACE().info("'%s' : '%s'", _name.c_str(), message.str().c_str());
}
