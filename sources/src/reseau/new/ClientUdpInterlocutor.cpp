/*
 * ClientUdpInterlocutor.cpp
 *
 *  Created on: 20 juin 2013
 *      Author: vgdj7997
 */

#include <sstream>
#include <iostream>
#include <string>

using namespace std;

#include "SDL.h"
#include "SDL_net.h"

#include "reseau/new/message/TechnicalMessage.h"
#include "reseau/new/message/S2CConnectionAcceptedTechnicalMessage.h"
#include "reseau/new/message/C2SHelloTechnicalMessage.h"
#include "reseau/new/message/C2SByeTechnicalMessage.h"

#include "reseau/new/ClientUdpInterlocutor.h"

ClientUdpInterlocutor::ClientUdpInterlocutor(Uint16 localPort) : TechnicalInterlocutor(localPort) {
	_interlocutor = NULL;
	_distantIp = "";
	_distantPort = 0;
	_socket = 0;
	_socketSet = 0;
	_packetIn = 0;
	_packetOut = 0;
	_tryConnectionLastTime = 0;
	_tryConnectionNumber = 0;

	setConnexionStatus(STOPPED);
}

ClientUdpInterlocutor::~ClientUdpInterlocutor() {
	close();
}

void ClientUdpInterlocutor::connect(Interlocutor2* interlocutor, const string& distantIp, Uint16 distantPort) throw(ConnectionFailedException) {
	_interlocutor = interlocutor;
	_distantIp = distantIp;
	_distantPort = distantPort;

	_packetIn = SDLNet_AllocPacket(65535);
	_packetOut = SDLNet_AllocPacket(65535);

	_interlocutor->setCondIntelligence(getCondIntelligence());

	try {
		// Résolution adresse distant
		if( SDLNet_ResolveHost(&_distantAddress, _distantIp.c_str(), distantPort) ) {
			stringstream msg;
			msg << "SDLNet_ResolveHost - UDP connection, erreur de resolution adresse distant : " << SDLNet_GetError();
			throw ConnectionFailedException(msg.str());
		}

		// Ouverture socket
		_socket = SDLNet_UDP_Open( getLocalPort() );

		if(!_socket) {
			stringstream msg;
			msg << "SDLNet_UDP_Open - UDP connection, erreur d'ouverture du socket : " << SDLNet_GetError();
			throw ConnectionFailedException(msg.str());
		}

		// Attachement socket / adresse
		_packetIn->channel = SDLNet_UDP_Bind( _socket, -1, &_distantAddress );
		_packetOut->channel = _packetIn->channel;

		if(_packetIn->channel == -1) {
			stringstream msg;
			msg << "SDLNet_UDP_Bind - UDP connection, erreur d'ouverture du socket : " << SDLNet_GetError();
			throw ConnectionFailedException(msg.str());
		}

		_socketSet = SDLNet_AllocSocketSet(1);

		// Allocation d'un socket set qui permettra d'attendre et de scruter l'arrivée de données
		if(!_socketSet) {
			stringstream msg;
			msg << "SDLNet_AllocSocketSet - UDP connection, erreur de reservation du socket set : " << SDLNet_GetError();
			throw ConnectionFailedException(msg.str());
		}

		// Attachement du socket au socket set
		int addSocket = SDLNet_UDP_AddSocket(_socketSet, _socket);

		if(addSocket == -1) {
			stringstream msg;
			msg << "SDLNet_UDP_AddSocket - UDP connection, erreur d'attachement du socket au socket set : " << SDLNet_GetError();
			throw ConnectionFailedException(msg.str());
		}

		_tryConnectionNumber = 0;
		_tryConnectionLastTime = 0;

		setConnexionStatus(CONNECTING);
	}
	catch(ConnectionFailedException& exception) {
		cerr << endl << exception.getMessage();

		close();

		throw exception;
	}
}

void ClientUdpInterlocutor::close() {
	setConnexionStatus(STOPPING);

	// Libération du socket set
	if(_socketSet) {
		SDLNet_FreeSocketSet(_socketSet);
		_socketSet = 0;
	}

	// Fermeture du socket
	if(_socket) {
		SDLNet_UDP_Close(_socket);
		_socket = 0;
	}

	// Libération des paquets
	if(_packetIn) {
		SDLNet_FreePacket(_packetIn);
		_packetIn = 0;
	}

	if(_packetOut) {
		SDLNet_FreePacket(_packetOut);
		_packetOut = 0;
	}

	setConnexionStatus(STOPPED);
}

const string& ClientUdpInterlocutor::getDistantIp() const {
	return _distantIp;
}

Uint16 ClientUdpInterlocutor::getDistantPort() const {
	return _distantPort;
}

void ClientUdpInterlocutor::manageConnection(TechnicalMessage* lastConnectionMsg) {
	Uint32 currentTime = SDL_GetTicks();

	if(lastConnectionMsg && (TechnicalMessage::S2C_CONNECTION_ACCEPTED == lastConnectionMsg->getCode())) {
//		S2CConnectionAcceptedTechnicalMessage* msg = (S2CConnectionAcceptedTechnicalMessage*)lastConnectionMsg;
//		int port = msg->getPort();



		setConnexionStatus(CONNECTED);
	}
	if(lastConnectionMsg && (TechnicalMessage::S2C_CONNECTION_REFUSED == lastConnectionMsg->getCode())) {
		close();
	}
	else if(_tryConnectionNumber > 10) {
		close();
	}
	else if(currentTime - _tryConnectionLastTime > 2000) {
		C2SHelloTechnicalMessage msg;
		_interlocutor->addTechnicalMessageToSend(msg.toBytes());
		_tryConnectionLastTime = currentTime;
		_tryConnectionNumber++;
	}
}

void ClientUdpInterlocutor::intelligenceProcess() {
	while(STOPPED != getConnexionStatus()) {
		SDL_CondWaitTimeout(getCondIntelligence(), getMutexIntelligence(), 1000);

		char* msg;

		TechnicalMessage* lastConnectionMsg = NULL;

		while((msg = _interlocutor->getTechnicalMessageReceived())) {
			TechnicalMessage* techMsg = TechnicalMessage::traduct(msg);
			delete[] msg;

			if(techMsg) {
				switch(techMsg->getCode()) {
				case TechnicalMessage::S2C_CONNECTION_ACCEPTED:
				case TechnicalMessage::S2C_CONNECTION_REFUSED:
					lastConnectionMsg = techMsg;
					break;

				case TechnicalMessage::S2C_DISCONNECTION:
					break;
				}
			}
			else {
				// unkown message => ignore it
			}
		}

		if(CONNECTING == getConnexionStatus()) {
			manageConnection(lastConnectionMsg);
		}

		if(STOPPING == getConnexionStatus()) {
			C2SByeTechnicalMessage msg;
			_interlocutor->addTechnicalMessageToSend(msg.toBytes());

			setConnexionStatus(STOPPED);
		}
	}

	cout << endl << "Stop intelligence process";
}

void ClientUdpInterlocutor::sendingProcess() {

	while(STOPPED != getConnexionStatus()) {
		_interlocutor->waitDataToSend(1000);
		char* data;

		// Envoi les messages techniques tant qu'on est pas arrêté, sinon purge les
		while((data = _interlocutor->getTechnicalMessageToSend()))  {
			if(STOPPED != getConnexionStatus()) {
				_packetOut->len = sizeof(data);
				SDLNet_UDP_Send(_socket, _packetOut->channel, _packetOut);
			}

			delete[] data;
		}

		// Envoi les messages de données tant qu'on est connecté, sinon purge les
		while((data = _interlocutor->getDataToSend())) {
			if(CONNECTED == getConnexionStatus()) {
				_packetOut->len = sizeof(data);
				SDLNet_UDP_Send(_socket, _packetOut->channel, _packetOut);
			}

			delete[] data;
		}
	}

	cout << endl << "Stop sending process";
}

void ClientUdpInterlocutor::receiveOnePacket() {
	int packetReceived = SDLNet_UDP_Recv(_socket, _packetIn);

	if(packetReceived) {
		int size = _packetIn->len;

		if(size >= 2) {
			Uint16 code =  SDLNet_Read16(_packetIn->data);

			switch(code) {
			case TechnicalMessage::DATA:
				if(CONNECTED == getConnexionStatus()) {
					int offset = 2;
					char* data = new char[size - offset];
					memcpy(data, _packetIn->data + offset, size - offset);
					_interlocutor->addDataReceived(data);
				}
				break;

			default:
				if(STOPPED != getConnexionStatus()) {
					char* data = new char[size];
					memcpy(data, _packetIn->data, size);
					_interlocutor->addTechnicalMessageReceived(data);
				}
				break;
			}
		}
		else {
			// Message inconsistant => ignoré
		}
	}
}

void ClientUdpInterlocutor::receivingProcess() {
	while(STOPPED != getConnexionStatus()) {
		int socketReady = SDLNet_CheckSockets(_socketSet, 1000);

		if(STOPPED != getConnexionStatus()) {
			if(socketReady == -1) {
				stringstream msg;
				msg << "SDLNet_CheckSockets - UDP receive : " << SDLNet_GetError();
			}
			else if(socketReady > 0) {
				receiveOnePacket();
			}
		}
	}

	cout << endl << "Stop receiving process";
}
