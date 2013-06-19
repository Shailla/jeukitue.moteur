/*
 * TechnicalInterlocutor.cpp
 *
 *  Created on: 17 juin 2013
 *      Author: vgdj7997
 */

#include "sdl.h"

#include "reseau/new/TechnicalInterlocutor.h"

TechnicalInterlocutor::TechnicalInterlocutor(const string& ip, Uint16 port, Interlocutor& interlocutor) {
	_ip = ip;
	_port = port;
	_interlocutor = interlocutor;
	_status = CONNEXION_STATUS::STOPPED;

	SDL_CreateThread(startBoucle, (void*)this);
}

TechnicalInterlocutor::~TechnicalInterlocutor() {
}

int TechnicalInterlocutor::startBoucle(void* thiz) {
	return ((TechnicalInterlocutor*)thiz)->boucle();
}

const string& TechnicalInterlocutor::getIp() const {
	return _ip;
}

Uint16 TechnicalInterlocutor::getPort() const {
	return _port;
}

void TechnicalInterlocutor::setConnexionStatus(CONNEXION_STATUS status) {
	_status = status;
}

TechnicalInterlocutor::CONNEXION_STATUS TechnicalInterlocutor::getConnexionStatus() const {
	return _status;
}
