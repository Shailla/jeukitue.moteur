/*
 * TechnicalInterlocutor.cpp
 *
 *  Created on: 17 juin 2013
 *      Author: vgdj7997
 */

#include <string>
#include <sstream>

#include "SDL.h"

#include "util/Trace.h"
#include "reseau/new/message/TechnicalMessage.h"

#include "reseau/new/TechnicalInterlocutor.h"

using namespace std;

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
		log(__LINE__, __FILE__, "I am now STOPPED");
		break;
	case CONNECTING:
		log(__LINE__, __FILE__, "I am now CONNECTING");
		break;
	case CONNECTED:
		log(__LINE__, __FILE__,  "I am now CONNECTED");
		break;
	case STOPPING:
		log(__LINE__, __FILE__, "I am now STOPPING");
		break;
	default:
		stringstream message;
		message << "We should never be here " << __FILE__ << " " << __LINE__;
		log(__LINE__, __FILE__, message);
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

void TechnicalInterlocutor::log(int line, const char* sourceFile, const char* message) {
	Donnees(line, sourceFile).info("'%s' : '%s'", _name.c_str(), message);
}

void TechnicalInterlocutor::log(int line, const char* sourceFile, const string& message) {
	Donnees(line, sourceFile).info("'%s' : '%s'", _name.c_str(), message.c_str());
}

void TechnicalInterlocutor::log(int line, const char* sourceFile, const stringstream& message) {
	Donnees(line, sourceFile).info("'%s' : '%s'", _name.c_str(), message.str().c_str());
}
