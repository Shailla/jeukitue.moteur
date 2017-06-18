/*
 * TcpServer.h
 *
 *  Created on: 16 juin 2017
 *      Author: Erwin
 */

#ifndef SRC_RESEAU_TCP_TCPSERVER_H_
#define SRC_RESEAU_TCP_TCPSERVER_H_

#include <map>

#include "SDL.h"
#include "SDL_net.h"

#include "reseau/tcp/TcpPacket.h"
#include "reseau/tcp/TcpSession.h"

namespace jkt {

class TcpServer {
	static const int TCP_CLIENTS_SIZE = 20;
	static const int TCP_BUFFER_SIZE = 16384;
	static const int TCP_SOCKET_TIMEOUT = 180000;	// millisecondes

	/** Port du serveur TCP */
	int _serverPort;
	TCPsocket _serverSocket;
	std::map<TCPsocket, TcpSession> _clientSockets;
	SDLNet_SocketSet _socketSet;

	TcpPacket* receive(Uint32 time, TcpSession& session);

public:
	TcpServer(int port);
	virtual ~TcpServer();

	void start();
	void stop();

	TcpPacket* receive();
	void send(TcpSession* session, void* data, int size);
};

} /* namespace jkt */

#endif /* SRC_RESEAU_TCP_TCPSERVER_H_ */
