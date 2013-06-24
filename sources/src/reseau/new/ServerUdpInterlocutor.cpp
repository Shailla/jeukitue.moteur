/*
 * ServerUdpInterlocutor.cpp
 *
 *  Created on: 20 juin 2013
 *      Author: vgdj7997
 */

#include <sstream>
#include <iostream>
#include <string>
#include <map>

using namespace std;

#include "SDL.h"
#include "SDL_net.h"

#include "reseau/new/message/TechnicalMessage.h"
#include "reseau/new/message/S2CConnectionAcceptedTechnicalMessage.h"
#include "reseau/new/message/C2SHelloTechnicalMessage.h"
#include "reseau/new/message/C2SByeTechnicalMessage.h"

#include "reseau/new/ServerUdpInterlocutor.h"

ServerUdpInterlocutor::ServerUdpInterlocutor(Uint16 localPort) : TechnicalInterlocutor(localPort) {
	_interlocutor = NULL;
	_distantIp = "";
	_distantPort = 0;
	_socket = 0;
	_socketSet = 0;
	_packetIn = 0;
	_packetOut = 0;

	setConnexionStatus(STOPPED);
}

ServerUdpInterlocutor::~ServerUdpInterlocutor() {
	close();
}

void ServerUdpInterlocutor::connect() throw(ConnectionFailedException) {
	_packetIn = SDLNet_AllocPacket(65535);
	_packetOut = SDLNet_AllocPacket(65535);

	_interlocutor->setCondIntelligence(getCondIntelligence());

	try {
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

		setConnexionStatus(CONNECTED);
	}
	catch(ConnectionFailedException& exception) {
		cerr << endl << exception.getMessage();

		close();

		throw exception;
	}
}

void ServerUdpInterlocutor::close() {
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

void ServerUdpInterlocutor::manageConnection(const IPaddress& address, C2SHelloTechnicalMessage* msg) {
	map<IPaddress, ClientOfServer*>::iterator it = _clientsOfServer.find(address);


	/* **************************************************
	 * Enregistrement connexion client
	 * *************************************************/

	// Si le client n'est pas encore connecté alors enregistre sa connexion
	if(it == _clientsOfServer.end()) {
		Interlocutor2* clientInterlocutor = new Interlocutor2();
		ClientOfServer* client = new ClientOfServer(address, clientInterlocutor);

		client->setStatus(TechnicalInterlocutor::CONNECTED);
		_clientsOfServer[address] = client;

		cout << endl << "Server says : Client connected : " << address.port << ":" << address.host;
	}


	/* **************************************************
	 * Confirmation connexion client
	 * *************************************************/

	// S'il est maintenant connecté alors confirme au client qu'il est connecté (même s'il l'était déjà, peut-être n'a-t-il pas reçu la précédente confirmation)
	it = _clientsOfServer.find(address);

	if(it != _clientsOfServer.end()) {
		ClientOfServer* client = it->second;

		S2CConnectionAcceptedTechnicalMessage msg(getLocalPort());
		client->getInterlocutor()->pushTechnicalMessageToSend(msg.toBytes());
	}
}

void ServerUdpInterlocutor::manageDisconnection(const IPaddress& address, C2SByeTechnicalMessage* msg) {
	if(_clientsOfServer.find(address) == _clientsOfServer.end()) {
		cout << endl << "Server says : Client disconnected : " << address.port << ":" << address.host;
		_clientsOfServer.erase(address);
	}
}

void ServerUdpInterlocutor::intelligenceProcess() {
	while(CONNECTED == getConnexionStatus()) {
		SDL_CondWaitTimeout(getCondIntelligence(), getMutexIntelligence(), 1000);

		Interlocutor2::DataAddress* msg;

		// Gestion des événements liés aux clients non-connectés
		while((msg = _interlocutor->popTechnicalMessageReceived())) {
			TechnicalMessage* techMsg = TechnicalMessage::traduct(msg->getData());

			if(techMsg) {
				switch(techMsg->getCode()) {
				case TechnicalMessage::C2S_HELLO:
					manageConnection(msg->getAddress(), (C2SHelloTechnicalMessage*)techMsg);
					break;

				case TechnicalMessage::C2S_BYE:
					manageDisconnection(msg->getAddress(), (C2SByeTechnicalMessage*)techMsg);
					break;
				}
			}
			else {
				// unkown message => ignore it
			}

			delete msg;
			delete techMsg;
		}
	}

	cout << endl << "Server says : Stop intelligence process";
}

void ServerUdpInterlocutor::sendingProcess() {

	while(CONNECTED != getConnexionStatus()) {
		_interlocutor->waitDataToSend(1000);
		char* data;

		// Envoi les messages techniques tant que le serveur est connecté, sinon purge les
		while((data = _interlocutor->popTechnicalMessageToSend()))  {
			if(CONNECTED != getConnexionStatus()) {
				_packetOut->len = sizeof(data);
				SDLNet_UDP_Send(_socket, _packetOut->channel, _packetOut);
			}

			delete[] data;
		}

		// Envoi les messages de données tant que le serveur est connecté, sinon purge les
		while((data = _interlocutor->popDataToSend())) {
			if(CONNECTED == getConnexionStatus()) {
				_packetOut->len = sizeof(data);
				SDLNet_UDP_Send(_socket, _packetOut->channel, _packetOut);
			}

			delete[] data;
		}
	}

	cout << endl << "Server says : Stop sending process";
}

void ServerUdpInterlocutor::receiveOnePacket() {
	int packetReceived = SDLNet_UDP_Recv(_socket, _packetIn);

	if(CONNECTED == getConnexionStatus() && packetReceived) {
		IPaddress address = _packetIn->address;
		int size = _packetIn->len;

		if(_clientsOfServer.find(address) == _clientsOfServer.end()) {
			// Gestion des clients non-connectés
			if(size >= 2) {
				Uint16 code =  SDLNet_Read16(_packetIn->data);

				switch(code) {
				case TechnicalMessage::C2S_HELLO:
				case TechnicalMessage::C2S_BYE:
					char* data = new char[size];
					memcpy(data, _packetIn->data, size);
					_interlocutor->pushTechnicalMessageReceived(address, data);
					break;
				}
			}
			else {
				// Message inconsistant => ignoré
			}
		}
		else {
			// Gestion des clients connectés
			ClientOfServer* client = _clientsOfServer[address];

			if(size >= 2) {
				Uint16 code =  SDLNet_Read16(_packetIn->data);

				switch(code) {
				case TechnicalMessage::DATA:
				{
					int offset = 2;
					char* data = new char[size - offset];
					memcpy(data, _packetIn->data + offset, size - offset);
					client->getInterlocutor()->pushDataReceived(data);
					break;
				}

				default:
				{
					char* data = new char[size];
					memcpy(data, _packetIn->data, size);
					client->getInterlocutor()->pushTechnicalMessageReceived(address, data);
					break;
				}
				}
			}
			else {
				// Message inconsistant => ignoré
			}
		}
	}
}

void ServerUdpInterlocutor::receivingProcess() {
	while(CONNECTED == getConnexionStatus()) {
		int socketReady = SDLNet_CheckSockets(_socketSet, 1000);

		if(socketReady == -1) {
			cerr << "SDLNet_CheckSockets - UDP receive : " << SDLNet_GetError();
		}
		else if(socketReady > 0) {
			receiveOnePacket();
		}
	}

	cout << endl << "Server says : Stop receiving process";
}
