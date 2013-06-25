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

using namespace JktUtils;

#include "reseau/new/Interlocutor2.h"
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

	_interlocutor->setCondIntelligence(getCondIntelligence());

	_packetIn = SDLNet_AllocPacket(65535);
	_packetOut = SDLNet_AllocPacket(65535);

	try {
		// R�solution adresse distant
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

		// Allocation d'un socket set qui permettra d'attendre et de scruter l'arriv�e de donn�es
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
}

void ClientUdpInterlocutor::close() {
	setConnexionStatus(STOPPING);

	waitProcessesFinished();

	// Lib�ration du socket set
	if(_socketSet) {
		SDLNet_FreeSocketSet(_socketSet);
		_socketSet = 0;
	}

	// Fermeture du socket
	if(_socket) {
		SDLNet_UDP_Close(_socket);
		_socket = 0;
	}

	// Lib�ration des paquets
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

			cout << endl << "Client says : Connection accepted" << flush;
			setConnexionStatus(CONNECTED);

			break;
		}
		case TechnicalMessage::S2C_CONNECTION_REFUSED:
		{
			cout << endl << "Client says : Connection refused" << flush;
			close();
			break;
		}
		}
	}

	if(_tryConnectionNumber > 10) {
		close();
	}
	else if(currentTime - _tryConnectionLastTime > 2000) {
		cout << endl << "Client says : Asking connection" << flush;

		C2SHelloTechnicalMessage msg;
		_interlocutor->pushTechnicalMessageToSend(msg.toBytes());
		_tryConnectionLastTime = currentTime;
		_tryConnectionNumber++;
	}
}

void ClientUdpInterlocutor::intelligenceProcess() {
	while(STOPPED != getConnexionStatus() && STOPPING != getConnexionStatus()) {
		SDL_CondWaitTimeout(getCondIntelligence(), getMutexIntelligence(), 1000);

		Interlocutor2::DataAddress* msg;

		TechnicalMessage* lastConnectionMsg = NULL;

		while((msg = _interlocutor->popTechnicalMessageReceived())) {
			TechnicalMessage* techMsg = TechnicalMessage::traduct(msg->getBytes());

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
	}

	cout << endl << "Client says : Stop intelligence process" << flush;
}

void ClientUdpInterlocutor::sendingProcess() {
	while(STOPPED != getConnexionStatus() && STOPPING != getConnexionStatus()) {
		_interlocutor->waitDataToSend(1000);
		Interlocutor2::DataAddress* data;

		// Envoi les messages techniques tant qu'on est pas arr�t�, sinon purge les
		while((data = _interlocutor->popTechnicalMessageToSend()))  {
			if(STOPPED != getConnexionStatus()) {
				_packetOut->len = data->getBytes()->size();
				memcpy(_packetOut->data, data->getBytes()->getBytes(), _packetOut->len);
				int result = SDLNet_UDP_Send(_socket, _packetOut->channel, _packetOut);

				cout << endl << "Client says : Sending some technical message of size and result " << data->getBytes()->size() << " - " << result;
			}

			delete data;
		}

		// Envoi les messages de donn�es tant qu'on est connect�, sinon purge les
		while((data = _interlocutor->popDataToSend())) {
			if(CONNECTED == getConnexionStatus()) {
				cout << endl << "Client says : Sending some data message";

				_packetOut->len = data->getBytes()->size();
				memcpy(_packetOut->data, data->getBytes()->getBytes(), _packetOut->len);
				SDLNet_UDP_Send(_socket, _packetOut->channel, _packetOut);
			}

			delete data;
		}
	}

	cout << endl << "Client says : Stop sending process" << flush;
}

void ClientUdpInterlocutor::receiveOnePacket() {
	int packetReceived = SDLNet_UDP_Recv(_socket, _packetIn);

	if(packetReceived) {
		int size = _packetIn->len;
		IPaddress address = _packetIn->address;

		if(size >= 2) {
			Uint16 code =  SDLNet_Read16(_packetIn->data);

			switch(code) {
			case TechnicalMessage::DATA:
				if(CONNECTED == getConnexionStatus()) {
					int offset = 2;
					char* data = new char[size - offset];
					memcpy(data, _packetIn->data + offset, size - offset);
					cout << endl << "Client says : Received data packet";
					_interlocutor->pushDataReceived(address, new Bytes(data, size - offset));
				}
				break;

			default:
				if(STOPPED != getConnexionStatus()) {
					char* data = new char[size];
					memcpy(data, _packetIn->data, size);
					cout << endl << "Client says : Received packet supposed technical";
					_interlocutor->pushTechnicalMessageReceived(address, new Bytes(data, size));
				}
				break;
			}
		}
		else {
			// Message inconsistant => ignor�
		}
	}
}

void ClientUdpInterlocutor::receivingProcess() {
	while(STOPPED != getConnexionStatus() && STOPPING != getConnexionStatus()) {
		int socketReady = SDLNet_CheckSockets(_socketSet, 1000);

		if(STOPPED != getConnexionStatus()) {
			if(socketReady == -1) {
				cerr << endl << "Client SDLNet_CheckSockets - UDP receive : " << SDLNet_GetError();
			}
			else if(socketReady > 0) {
				receiveOnePacket();
			}
		}
	}

	cout << endl << "Client says : Stop receiving process" << flush;
}