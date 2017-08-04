/*
 * TcpServer.h
 *
 *  Created on: 16 juin 2017
 *      Author: Erwin
 */

#ifndef SRC_RESEAU_TCP_TCPSERVER_H_
#define SRC_RESEAU_TCP_TCPSERVER_H_

#include <vector>
#include <map>

#include "SDL.h"
#include "SDL_net.h"

#include "reseau/tcp/TcpPacket.h"
#include "reseau/tcp/TcpSession.h"

namespace jkt {

class TcpServer {
	int _serverPort;		// Port du serveur TCP
	int _tcpSocketTimeout;	// millisecondes
	int _tcpClientsSize;
	int _tcpBufferSize;

	TCPsocket _serverSocket;
	std::map<TCPsocket, TcpSession> _clientSockets;
	SDLNet_SocketSet _socketSet;

	int _acknowledgementCount;

	TcpPacket* receive(Uint32 now, TcpSession& session);

public:
	TcpServer(int port);
	virtual ~TcpServer();

	void start();
	void stop();

	void setTcpBufferSize(int tcpBufferSize);
	void setTimeout(int tcpSocketTimeout);
	void setClientsSize(int tcpClientsSize);

	std::vector<TcpPacket*> receive(long maxTime);
	void send(TcpSession* session, void* data, int size);

	long getAcknowledgementCount() const;
};

} /* namespace jkt */

#endif /* SRC_RESEAU_TCP_TCPSERVER_H_ */
