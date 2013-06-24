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

#include "reseau/new/exception/NotConnectedException.h"
#include "reseau/new/exception/ConnectionFailedException.h"
#include "reseau/new/TechnicalInterlocutor.h"

bool operator < (const IPaddress& adr1, const IPaddress& adr2) {
	if(adr1.host != adr2.host) {
		return adr1.host < adr2.host;
	}
	else {
		return adr1 < adr2;
	}
}

class ServerUdpInterlocutor : TechnicalInterlocutor {
	class ClientOfServer {
		IPaddress _address;
		Interlocutor2* _interlocutor;
		CONNEXION_STATUS _status;

	public:
		ClientOfServer(const IPaddress& address, Interlocutor2* interlocutor) {
			_address = address;
			_interlocutor = interlocutor;
			_status = TechnicalInterlocutor::STOPPED;
		}

		Interlocutor2* getInterlocutor() {
			return _interlocutor;
		}

		TechnicalInterlocutor::CONNEXION_STATUS getStatus() const {
			return _status;
		}

		void setStatus(TechnicalInterlocutor::CONNEXION_STATUS status) {
			_status = status;
		}

		IPaddress getIpAddress() const {
			return _address;
		}
	};

	string _distantIp;
	Uint16 _distantPort;
	Interlocutor2* _interlocutor;

	IPaddress _distantAddress;
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
	void connect() throw(ConnectionFailedException);
};

#endif /* SERVERINTERLOCUTOR_H_ */
