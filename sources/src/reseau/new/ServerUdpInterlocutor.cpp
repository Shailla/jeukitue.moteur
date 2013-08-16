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

#include "util/IpUtils.h"
#include "reseau/new/message/TechnicalMessage.h"
#include "reseau/new/message/C2SByeTechnicalMessage.h"
#include "reseau/new/message/C2SHelloTechnicalMessage.h"
#include "reseau/new/message/S2CConnectionAcceptedTechnicalMessage.h"
#include "reseau/new/Interlocutor2.h"

#include "reseau/new/ServerUdpInterlocutor.h"

using namespace JktUtils;

bool operator < (const IPaddress& adr1, const IPaddress& adr2) {
	if(adr1.host != adr2.host) {
		return adr1.port < adr2.port;
	}
	else {
		return adr1.host < adr2.host;
	}
}

ServerUdpInterlocutor::ServerUdpInterlocutor(Uint16 localPort) : TechnicalInterlocutor(localPort) {
	_notConnectedInterlocutor = NULL;
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

NotConnectedInterlocutor2* ServerUdpInterlocutor::connect() throw(ConnectionFailedException) {
	_notConnectedInterlocutor = new NotConnectedInterlocutor2();
	_notConnectedInterlocutor->setCondIntelligence(getCondIntelligence());

	_packetIn = SDLNet_AllocPacket(65535);
	_packetOut = SDLNet_AllocPacket(65535);

	try {
		// Ouverture socket
		_socket = SDLNet_UDP_Open( getLocalPort() );

		if(!_socket) {
			stringstream msg;
			msg << "SDLNet_UDP_Open - UDP connection, erreur d'ouverture du socket : " << SDLNet_GetError();
			throw ConnectionFailedException(msg.str());
		}

		// Allocation d'un socket set qui permettra d'attendre et de scruter l'arrivée de données
		_socketSet = SDLNet_AllocSocketSet(1);

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
		cout << endl << "Serveur connecte";

		startProcesses();
		cout << endl << "Processus du serveur lances";
	}
	catch(ConnectionFailedException& exception) {
		cerr << endl << exception.getMessage();

		close();

		throw exception;
	}

	return _notConnectedInterlocutor;
}

void ServerUdpInterlocutor::close() {
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

void ServerUdpInterlocutor::manageConnection(const IPaddress& address, C2SHelloTechnicalMessage* msg) {
	map<IPaddress, ClientOfServer*>::iterator it = _clientsOfServer.find(address);


	/* **************************************************
	 * Enregistrement connexion client
	 * *************************************************/

	// Si le client n'est pas encore connecté alors enregistre sa connexion
	if(it == _clientsOfServer.end()) {
		Interlocutor2* clientInterlocutor = new Interlocutor2();
		ClientOfServer* client = new ClientOfServer(address, clientInterlocutor);

		client->setConnexionStatus(TechnicalInterlocutor::CONNECTED);
		_clientsOfServer[address] = client;
		_notConnectedInterlocutor->pushNewInterlocutor(clientInterlocutor);
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

		cout << endl << "Server says : Send connection accepted message";
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
		SDL_LockMutex(getMutexIntelligence());

		SDL_CondWaitTimeout(getCondIntelligence(), getMutexIntelligence(), 1000);


		/* ***********************************************************
		 * Gestion des clients non-connectés
		 * ***********************************************************/

		{
			DataAddress* msg;

			while((msg = _notConnectedInterlocutor->popTechnicalMessageReceived())) {
				TechnicalMessage* techMsg = TechnicalMessage::traduct(msg->getBytes());
				IPaddress address = msg->getAddress();

				if(techMsg) {
					switch(techMsg->getCode()) {
					case TechnicalMessage::C2S_HELLO:
						cout << endl << "Server says : C2S_HELLO received from " << IpUtils::translateAddress(address);
						manageConnection(address, (C2SHelloTechnicalMessage*)techMsg);
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


		/* ***********************************************************
		 * Gestion des clients connectés
		 * ***********************************************************/

		{
			JktUtils::Bytes* msg;

			map<IPaddress, ClientOfServer*>::iterator iter;

			for(iter = _clientsOfServer.begin() ; iter != _clientsOfServer.end() ; iter++) {
				IPaddress address = iter->first;
				ClientOfServer* client = iter->second;

				// Gestion des événements liés aux clients non-connectés
				while((msg = client->getInterlocutor()->popTechnicalMessageReceived())) {
					TechnicalMessage* techMsg = TechnicalMessage::traduct(msg);

					if(techMsg) {
						switch(techMsg->getCode()) {
						case TechnicalMessage::C2S_HELLO:
							manageConnection(address, (C2SHelloTechnicalMessage*)techMsg);
							break;

						case TechnicalMessage::C2S_BYE:
							cout << endl << "Server says : C2S_BYE received";
							manageDisconnection(address, (C2SByeTechnicalMessage*)techMsg);
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
		}

		SDL_UnlockMutex(getMutexIntelligence());
	}

	SDL_UnlockMutex(getMutexIntelligence());

	cout << endl << "Server says : Stop intelligence process";
}

void ServerUdpInterlocutor::sendingProcess() {

	while(CONNECTED == getConnexionStatus()) {
		_notConnectedInterlocutor->waitDataToSend(1000);


		/* *********************************************
		 * Gestion des envois aux clients non-connectés
		 * *********************************************/

		{
			DataAddress* data;

			// Envoi les messages techniques tant que le serveur est connecté, sinon purge les
			while((data = _notConnectedInterlocutor->popTechnicalMessageToSend()))  {
				if(CONNECTED == getConnexionStatus()) {
					_packetOut->len = data->getBytes()->size();
					memcpy(_packetOut->data, data->getBytes()->getBytes(), _packetOut->len);
					_packetOut->address = data->getAddress();
					SDLNet_UDP_Send(_socket, -1, _packetOut);
					cout << endl << "Envoi de donnees techniques en mode non-connecte a " << IpUtils::translateAddress(_packetOut->address);
				}

				delete data;
			}
		}


		/* *********************************************
		 * Gestion des envois aux clients connectés
		 * *********************************************/

		{
			JktUtils::Bytes* data;

			map<IPaddress, ClientOfServer*>::iterator iter;

			for(iter = _clientsOfServer.begin() ; iter != _clientsOfServer.end() ; iter++) {
				ClientOfServer* client = iter->second;

				// Envoi les messages techniques tant que le serveur est connecté, sinon purge les
				while((data = client->getInterlocutor()->popTechnicalMessageToSend()))  {
					if(CONNECTED == getConnexionStatus() && CONNECTED == client->getConnexionStatus()) {
						_packetOut->len = data->size();
						memcpy(_packetOut->data, data->getBytes(), _packetOut->len);
						_packetOut->address = client->getIpAddress();
						SDLNet_UDP_Send(_socket, -1, _packetOut);
						cout << endl << "Envoi de donnees techniques en mode connecte a " << IpUtils::translateAddress(_packetOut->address);
					}

					delete data;
				}

				// Envoi les messages de données tant que le serveur est connecté, sinon purge les
				while((data = client->getInterlocutor()->popDataToSend())) {
					if(CONNECTED == getConnexionStatus() && CONNECTED == client->getConnexionStatus()) {
						cout << endl << "Server says : Sending some data message";

						_packetOut->len = data->size();
						memcpy(_packetOut->data, data->getBytes(), _packetOut->len);
						_packetOut->address = client->getIpAddress();
						SDLNet_UDP_Send(_socket, -1, _packetOut);
						cout << endl << "Envoi de donnees a " << IpUtils::translateAddress(_packetOut->address);
					}

					delete data;
				}
			}
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
					_notConnectedInterlocutor->pushTechnicalMessageReceived(address, new JktUtils::Bytes(data, size));
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
					client->getInterlocutor()->pushDataReceived(new JktUtils::Bytes(data, size - offset));
					break;
				}

				default:
				{
					char* data = new char[size];
					memcpy(data, _packetIn->data, size);
					client->getInterlocutor()->pushTechnicalMessageReceived(new JktUtils::Bytes(data, size));
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

		if(socketReady > 0) {
			receiveOnePacket();
		}
	}

	cout << endl << "Server says : Stop receiving process";
}
