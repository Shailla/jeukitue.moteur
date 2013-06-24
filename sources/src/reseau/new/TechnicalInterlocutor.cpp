/*
 * TechnicalInterlocutor.cpp
 *
 *  Created on: 17 juin 2013
 *      Author: vgdj7997
 */

#include "SDL.h"

#include "reseau/new/message/TechnicalMessage.h"

#include "reseau/new/TechnicalInterlocutor.h"

TechnicalInterlocutor::TechnicalInterlocutor(Uint16 portLocal) {
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
	SDL_WaitThread(_threadReceiving, &status);
	SDL_WaitThread(_threadReceiving, &status);
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
