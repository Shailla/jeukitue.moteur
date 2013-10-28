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

ServerUdpInterlocutor::ServerUdpInterlocutor(const string& name, Uint16 localPort) : TechnicalInterlocutor(name, localPort), _clientsOfServer() {
	_notConnectedInterlocutor = NULL;
	_distantIp = "";
	_distantPort = 0;
	_socket = 0;
	_socketSet = 0;
	_packetIn = 0;
	_packetOut = 0;
	_sendingMutex = SDL_CreateMutex();
	_sendingCondition = SDL_CreateCond();
	_clientsOfServerMutex = SDL_CreateMutex();
}

ServerUdpInterlocutor::~ServerUdpInterlocutor() {
	close();
}

NotConnectedInterlocutor2* ServerUdpInterlocutor::connect() throw(ConnectionFailedException) {
	_notConnectedInterlocutor = new NotConnectedInterlocutor2(_sendingCondition, _sendingMutex);
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

		startProcesses();
		log("Les processus sont lances");
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
	SDL_LockMutex(_clientsOfServerMutex);

	map<IPaddress, ClientOfServer*>::iterator clientIt = _clientsOfServer.find(address);


	/* **************************************************
	 * Enregistrement connexion client
	 * *************************************************/

	// Si le client n'est pas encore connecté alors enregistre sa connexion
	if(clientIt == _clientsOfServer.end()) {
		Interlocutor2* clientInterlocutor = new Interlocutor2(_sendingCondition, _sendingMutex);
		ClientOfServer* client = new ClientOfServer(address, clientInterlocutor);

		client->setConnexionStatus(TechnicalInterlocutor::CONNECTED);
		_clientsOfServer[address] = client;
		_notConnectedInterlocutor->pushNewInterlocutor(clientInterlocutor);

		stringstream message;
		message << "This client is now connected : " << IpUtils::translateAddress(address);
		log(message);

		// Confirmation connexion client
		S2CConnectionAcceptedTechnicalMessage msg(getLocalPort());
		client->getInterlocutor()->pushTechnicalMessageToSend(msg.toBytes());

		log("Send 'connection accepted' message");
	}
	else {
		stringstream message;
		message << "This client is already connected : " << IpUtils::translateAddress(address);
		log(message);
	}

	SDL_UnlockMutex(_clientsOfServerMutex);
}

void ServerUdpInterlocutor::intelligenceProcess() {
	bool firstExecution = true;

	while(CONNECTED == getConnexionStatus()) {
		SDL_LockMutex(getMutexIntelligence());

		if(!firstExecution) {
			SDL_CondWaitTimeout(getCondIntelligence(), getMutexIntelligence(), 1000);
		}
		else {
			firstExecution = false;
		}

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
						stringstream message;
						message << "C2S_HELLO received from " << IpUtils::translateAddress(address);
						log(message);

						manageConnection(address, (C2SHelloTechnicalMessage*)techMsg);
						break;
					}
				}
				else {
					stringstream message;
					message << "Unknown not connected technical message received from " << IpUtils::translateAddress(address) << " => ignored";
					log(message);
				}

				delete msg;
				delete techMsg;
			}
		}


		/* ***********************************************************
		 * Gestion des clients connectés
		 * ***********************************************************/

		{
			SDL_LockMutex(_clientsOfServerMutex);

			JktUtils::Bytes* msg;

			bool incrementIter = true;
			map<IPaddress, ClientOfServer*>::iterator iter = _clientsOfServer.begin();

			while(iter != _clientsOfServer.end()) {
				IPaddress address = iter->first;
				ClientOfServer* client = iter->second;

				bool c2s_hello_received = false;
				bool c2s_bye_received = false;

				// Gestion des événements liés aux clients non-connectés
				while((msg = client->getInterlocutor()->popTechnicalMessageReceived())) {
					TechnicalMessage* techMsg = TechnicalMessage::traduct(msg);

					if(techMsg) {
						switch(techMsg->getCode()) {
						case TechnicalMessage::C2S_HELLO:
						{
							stringstream message1;
							message1 << "C2S_HELLO received from connected client : " << IpUtils::translateAddress(address);
							log(message1);

							c2s_hello_received = true;
							break;
						}
						case TechnicalMessage::C2S_BYE:
						{
							stringstream message1;
							message1 << "C2S_BYE received from connected client : " << IpUtils::translateAddress(address);
							log(message1);

							c2s_bye_received = true;
							break;
						}
						}
					}
					else {
						stringstream message;
						message << "Unknown connected technical message received from " << IpUtils::translateAddress(address) << " => ignored";
						log(message);
					}

					delete msg;
					delete techMsg;
				}

				if(c2s_bye_received) {
					// One or more C2S_BYE messages have been received for a client who is connected, disconnect it
					_clientsOfServer.erase(iter++);		// Need post-increment to manage std::map removing idiom
					incrementIter = false;				// Do not increment a second time the iterator

					stringstream message2;
					message2 << "Client is now disconnected : " << IpUtils::translateAddress(address);
					log(message2);
				}
				else if(c2s_hello_received) {
					// One or more C2S_HELLO messages have been received for a client which is connected, perhaps he has just been connected or not, in any case send a connection accepted confirmation
					stringstream message2;
					message2 << "Send 'connection accepted' message : "  << IpUtils::translateAddress(address);
					log(message2);

					S2CConnectionAcceptedTechnicalMessage msg(getLocalPort());
					client->getInterlocutor()->pushTechnicalMessageToSend(msg.toBytes());
				}

				if(incrementIter) {
					++iter;
				}
				else {
					incrementIter = true;
				}
			}

			SDL_UnlockMutex(_clientsOfServerMutex);
		}

		SDL_UnlockMutex(getMutexIntelligence());
	}

	log("Stop intelligence process");
}

void ServerUdpInterlocutor::sendingProcess() {
	bool firstExecution = true;

	while(CONNECTED == getConnexionStatus()) {
		if(!firstExecution) {
			_notConnectedInterlocutor->waitDataToSend(1000);	// _notConnectedInterlocutor uses the same sending mutex and condition than the clients of the server interlocutor
		}
		else {
			firstExecution = false;
		}


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

					stringstream message;
					message << "Envoi de donnees techniques en mode non-connecte a " << IpUtils::translateAddress(_packetOut->address);
					log(message);
				}

				delete data;
			}
		}


		/* *********************************************
		 * Gestion des envois aux clients connectés
		 * *********************************************/

		{
			SDL_LockMutex(_clientsOfServerMutex);

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

						stringstream message;
						message << "Envoi de donnees techniques en mode connecte a " << IpUtils::translateAddress(_packetOut->address);
						log(message);
					}

					delete data;
				}

				// Envoi les messages de données tant que le serveur est connecté, sinon purge les
				while((data = client->getInterlocutor()->popDataToSend())) {
					if(CONNECTED == getConnexionStatus() && CONNECTED == client->getConnexionStatus()) {
						_packetOut->len = data->size() + 2;

						SDLNet_Write16(TechnicalMessage::TECHNICAL_MESSAGE::DATA, _packetOut->data);

						memcpy(_packetOut->data + 2, data->getBytes(), _packetOut->len);
						_packetOut->address = client->getIpAddress();
						SDLNet_UDP_Send(_socket, -1, _packetOut);

						stringstream message;
						message << "Envoi de donnees user a " << IpUtils::translateAddress(_packetOut->address);
						log(message);
					}

					delete data;
				}
			}

			SDL_UnlockMutex(_clientsOfServerMutex);
		}
	}

	cout << endl << "Server says : Stop sending process";
}

void ServerUdpInterlocutor::receiveOnePacket() {
	int packetReceived = SDLNet_UDP_Recv(_socket, _packetIn);

	SDL_LockMutex(_clientsOfServerMutex);

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
				stringstream message;
				message << "Message inconsistant et client inconnu from " << IpUtils::translateAddress(address) << " ==> ignoré";
				log(message);
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

					stringstream message;
					message << "Reception de donnees user from " << IpUtils::translateAddress(address);
					log(message);
					break;
				}

				default:
				{
					char* data = new char[size];
					memcpy(data, _packetIn->data, size);
					client->getInterlocutor()->pushTechnicalMessageReceived(new JktUtils::Bytes(data, size));

					stringstream message;
					message << "Reception de donnees techniques from " << IpUtils::translateAddress(address);
					log(message);
					break;
				}
				}
			}
			else {
				stringstream message;
				message << "Message inconsistant de client connu from " << IpUtils::translateAddress(address) << " ==> ignoré";
				log(message);
			}
		}
	}

	SDL_UnlockMutex(_clientsOfServerMutex);
}

void ServerUdpInterlocutor::receivingProcess() {
	while(CONNECTED == getConnexionStatus()) {
		int socketReady = SDLNet_CheckSockets(_socketSet, 1000);

		if(socketReady > 0) {
			receiveOnePacket();
		}
	}


	log("Stop receiving process");
}
