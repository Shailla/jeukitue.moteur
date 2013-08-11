/*
 * ServerUdpInterlocutor.h
 *
 *  Created on: 20 juin 2013
 *      Author: vgdj7997
 */

#ifndef SERVERINTERLOCUTOR_H_
#define SERVERINTERLOCUTOR_H_

#include <string>
#include <map>

using namespace std;

#include "SDL.h"
#include "SDL_net.h"

#include "reseau/new/Interlocutor2.h"
#include "reseau/new/NotConnectedInterlocutor2.h"
#include "reseau/new/exception/NotConnectedException.h"
#include "reseau/new/exception/ConnectionFailedException.h"
#include "reseau/new/TechnicalInterlocutor.h"

class C2SHelloTechnicalMessage;
class C2SByeTechnicalMessage;

class ServerUdpInterlocutor : TechnicalInterlocutor {
	class ClientOfServer {
		IPaddress _address;
		Interlocutor2* _interlocutor;
		CONNEXION_STATUS _connexionStatus;

	public:
		ClientOfServer(const IPaddress& address, Interlocutor2* interlocutor) {
			_address = address;
			_interlocutor = interlocutor;
			_connexionStatus = TechnicalInterlocutor::STOPPED;
		}

		Interlocutor2* getInterlocutor() {
			return _interlocutor;
		}

		TechnicalInterlocutor::CONNEXION_STATUS getConnexionStatus() const {
			return _connexionStatus;
		}

		void setConnexionStatus(TechnicalInterlocutor::CONNEXION_STATUS connexionStatus) {
			_connexionStatus = connexionStatus;
		}

		IPaddress getIpAddress() const {
			return _address;
		}
	};

	string _distantIp;
	Uint16 _distantPort;
	NotConnectedInterlocutor2* _notConnectedInterlocutor;

	UDPsocket _socket;
	SDLNet_SocketSet _socketSet;
	UDPpacket* _packetIn;
	UDPpacket* _packetOut;

	map<IPaddress, ClientOfServer*> _clientsOfServer;

private:
	void receivingProcess();
	void sendingProcess();
	void intelligenceProcess();

	void receiveOnePacket();
	void manageConnection(const IPaddress& address, C2SHelloTechnicalMessage* msg);
	void manageDisconnection(const IPaddress& address, C2SByeTechnicalMessage* msg);

public:
	ServerUdpInterlocutor(Uint16 localPort);
	virtual ~ServerUdpInterlocutor();

	void close();
	NotConnectedInterlocutor2* connect() throw(ConnectionFailedException);
};

#endif /* SERVERINTERLOCUTOR_H_ */
