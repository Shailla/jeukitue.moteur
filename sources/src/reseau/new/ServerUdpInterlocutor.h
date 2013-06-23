/*
 * ServerUdpInterlocutor.h
 *
 *  Created on: 20 juin 2013
 *      Author: vgdj7997
 */

#ifndef SERVERINTERLOCUTOR_H_
#define SERVERINTERLOCUTOR_H_

#include <string>

using namespace std;

#include "SDL.h"
#include "SDL_net.h"

#include "reseau/new/exception/NotConnectedException.h"
#include "reseau/new/exception/ConnectionFailedException.h"
#include "reseau/new/TechnicalInterlocutor.h"

class ServerUdpInterlocutor : TechnicalInterlocutor {
	string _distantIp;
	Uint16 _distantPort;
	Interlocutor2* _interlocutor;

	IPaddress _distantAddress;
	UDPsocket _socket;
	SDLNet_SocketSet _socketSet;
	UDPpacket* _packetIn;
	UDPpacket* _packetOut;

	Uint32 _tryConnectionLastTime;
	int _tryConnectionNumber;

private:
	void receivingProcess();
	void sendingProcess();
	void intelligenceProcess();

	void receiveOnePacket();
	void manageConnection(TechnicalMessage* lastConnectionMsg);


public:
	ServerUdpInterlocutor(Uint16 localPort);
	virtual ~ServerUdpInterlocutor();

	void close();
	void connect() throw(ConnectionFailedException);
};

#endif /* SERVERINTERLOCUTOR_H_ */
