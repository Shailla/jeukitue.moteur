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

#include "util/types/Bytes.h"
#include "util/IpUtils.h"

using namespace JktUtils;

#include "reseau/new/message/TechnicalMessage.h"
#include "reseau/new/message/S2CConnectionAcceptedTechnicalMessage.h"
#include "reseau/new/message/C2SHelloTechnicalMessage.h"
#include "reseau/new/message/C2SByeTechnicalMessage.h"
#include "reseau/new/Interlocutor2.h"

#include "reseau/new/ClientUdpInterlocutor.h"

ClientUdpInterlocutor::ClientUdpInterlocutor(const string& name, Uint16 localPort) : TechnicalInterlocutor(name, localPort) {
	_interlocutor = NULL;
	_distantIp = "";
	_distantPort = 0;
	_socket = 0;
	_socketSet = 0;
	_packetIn = 0;
	_packetOut = 0;
	_tryConnectionLastTime = 0;
	_tryConnectionNumber = 0;
}

ClientUdpInterlocutor::~ClientUdpInterlocutor() {
	close();
}

Interlocutor2* ClientUdpInterlocutor::connect(const string& distantIp, Uint16 distantPort) throw(ConnectionFailedException) {
	_interlocutor = new Interlocutor2();
	_distantIp = distantIp;
	_distantPort = distantPort;

	_interlocutor->setCondIntelligence(getCondIntelligence());

	_packetIn = SDLNet_AllocPacket(65535);
	_packetOut = SDLNet_AllocPacket(65535);

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

		startProcesses();
	}
	catch(ConnectionFailedException& exception) {
		cerr << endl << exception.getMessage();

		close();

		throw exception;
	}

	return _interlocutor;
}

void ClientUdpInterlocutor::close() {
	setConnexionStatus(STOPPING);

	waitProcessesFinished();

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

	if(lastConnectionMsg) {
		switch(lastConnectionMsg->getCode()) {
		case TechnicalMessage::S2C_CONNECTION_ACCEPTED:
		{
			//			S2CConnectionAcceptedTechnicalMessage* msg = (S2CConnectionAcceptedTechnicalMessage*)lastConnectionMsg;
			//			int port = msg->getPort();

			log("Connection accepted received");
			setConnexionStatus(CONNECTED);

			break;
		}
		case TechnicalMessage::S2C_CONNECTION_REFUSED:
		{
			log("Connection refused received");
			close();
			break;
		}
		}
	}

	if(CONNECTING == getConnexionStatus()) {
		if(_tryConnectionNumber > 10) {
			stringstream message;
			message << "Server does not respond after " << (_tryConnectionNumber-1) << " connection requests sent";
			log(message);

			close();
		}
		else if(currentTime - _tryConnectionLastTime > 1000) {
			log("Asking connection");

			C2SHelloTechnicalMessage msg;
			_interlocutor->pushTechnicalMessageToSend(msg.toBytes());
			_tryConnectionLastTime = currentTime;
			_tryConnectionNumber++;
		}
	}
}

void ClientUdpInterlocutor::intelligenceProcess() {
	while(STOPPED != getConnexionStatus() && STOPPING != getConnexionStatus()) {
		SDL_LockMutex(getMutexIntelligence());

		SDL_CondWaitTimeout(getCondIntelligence(), getMutexIntelligence(), 1000);

		JktUtils::Bytes* msg;

		TechnicalMessage* lastConnectionMsg = NULL;

		while((msg = _interlocutor->popTechnicalMessageReceived())) {
			TechnicalMessage* techMsg = TechnicalMessage::traduct(msg);

			if(techMsg) {
				switch(techMsg->getCode()) {
				case TechnicalMessage::S2C_CONNECTION_ACCEPTED:
				case TechnicalMessage::S2C_CONNECTION_REFUSED:
					lastConnectionMsg = techMsg;
					techMsg = NULL;
					break;

				case TechnicalMessage::S2C_DISCONNECTION:
					break;
				}

				if(techMsg) {
					delete techMsg;
				}
			}
			else {
				// unkown message => ignore it
			}
		}

		if(CONNECTING == getConnexionStatus()) {
			manageConnection(lastConnectionMsg);

			if(lastConnectionMsg) {
				delete lastConnectionMsg;
			}
		}

		if(STOPPING == getConnexionStatus()) {
			// Send 3 disconnection messages to maximize the luck it arrive to destination (UDP !)
			C2SByeTechnicalMessage msg;
			_interlocutor->pushTechnicalMessageToSend(msg.toBytes());
			_interlocutor->pushTechnicalMessageToSend(msg.toBytes());
			_interlocutor->pushTechnicalMessageToSend(msg.toBytes());

			setConnexionStatus(STOPPED);
		}

		delete msg;
		SDL_UnlockMutex(getMutexIntelligence());
	}

	SDL_UnlockMutex(getMutexIntelligence());

	log("Stop intelligence process");
}

void ClientUdpInterlocutor::sendingProcess() {
	while(STOPPED != getConnexionStatus() && STOPPING != getConnexionStatus()) {
		_interlocutor->waitDataToSend(1000);
		JktUtils::Bytes* data;

		// Envoi les messages techniques tant qu'on est pas arrêté, sinon purge les
		while((data = _interlocutor->popTechnicalMessageToSend()))  {
			if(STOPPED != getConnexionStatus()) {
				_packetOut->len = data->size();
				memcpy(_packetOut->data, data->getBytes(), _packetOut->len);
				_packetOut->address = _distantAddress;
				SDLNet_UDP_Send(_socket, -1, _packetOut);

				stringstream message;
				message << "Envoi de donnees techniques a " << IpUtils::translateAddress(_packetOut->address);
				log(message);

			}

			delete data;
		}

		// Envoi les messages de données tant qu'on est connecté, sinon purge les
		while((data = _interlocutor->popDataToSend())) {
			if(CONNECTED == getConnexionStatus()) {
				_packetOut->len = data->size();
				memcpy(_packetOut->data, data->getBytes(), _packetOut->len);
				_packetOut->address = _distantAddress;
				SDLNet_UDP_Send(_socket, -1, _packetOut);

				stringstream message;
				message << "Client : Envoi de donnees user a " << IpUtils::translateAddress(_packetOut->address);
				log(message);
			}

			delete data;
		}
	}

	log("Stop sending process");
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
					_interlocutor->pushDataReceived(new Bytes(data, size - offset));
				}
				break;

			default:
				if(STOPPED != getConnexionStatus()) {
					char* data = new char[size];
					memcpy(data, _packetIn->data, size);
					_interlocutor->pushTechnicalMessageReceived(new Bytes(data, size));
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
	while(STOPPED != getConnexionStatus() && STOPPING != getConnexionStatus()) {
		int socketReady = SDLNet_CheckSockets(_socketSet, 1000);

		if(STOPPED != getConnexionStatus()) {
			if(socketReady <= -1) {
				cerr << endl << "Client SDLNet_CheckSockets - UDP receive : " << SDLNet_GetError();
				perror("SDLNet_CheckSockets");
			}
			else {
				receiveOnePacket();
			}
		}
	}

	log("Stop receiving process");
}
