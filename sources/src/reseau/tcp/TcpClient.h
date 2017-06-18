/*
 * TcpClient.h
 *
 *  Created on: 16 juin 2017
 *      Author: Erwin
 */

#ifndef SRC_RESEAU_TCP_TCPCLIENT_H_
#define SRC_RESEAU_TCP_TCPCLIENT_H_

#include "SDL.h"
#include "SDL_net.h"

namespace jkt {

class TcpClient {
	TCPsocket _socket;
	Uint32 _lastTime;

public:
	TcpClient(TCPsocket socket, Uint32 lastTime);
	virtual ~TcpClient();

	void setLastTime(Uint32 lastTime);
	Uint32 getLastTime() const;
	TCPsocket getSocket();
};

} /* namespace jkt */

#endif /* SRC_RESEAU_TCP_TCPCLIENT_H_ */
