/*
 * ClientInterlocutor.h
 *
 *  Created on: 20 juin 2013
 *      Author: vgdj7997
 */

#ifndef CLIENTINTERLOCUTOR_H_
#define CLIENTINTERLOCUTOR_H_

#include <string>

#include "SDL.h"
#include "SDL_net.h"

#include "reseau/new/exception/NotConnectedException.h"
#include "reseau/new/exception/ConnectionFailedException.h"
#include "reseau/new/TechnicalInterlocutor.h"

class TechnicalMessage;

class ClientUdpInterlocutor : public TechnicalInterlocutor {
	std::string _distantIp;
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
	ClientUdpInterlocutor(const std::string& name, Uint16 localPort);
	virtual ~ClientUdpInterlocutor();

	void close();
	Interlocutor2* connect(const std::string& distantIp, Uint16 distantPort) throw(ConnectionFailedException);
	const std::string& getDistantIp() const;
	Uint16 getDistantPort() const;
};

#endif /* CLIENTINTERLOCUTOR_H_ */
