/*
 * TcpClient.h
 *
 *  Created on: 16 juin 2017
 *      Author: Erwin
 */

#ifndef SRC_RESEAU_TCP_TCPSESSION_H_
#define SRC_RESEAU_TCP_TCPSESSION_H_

#include "SDL.h"
#include "SDL_net.h"

namespace jkt {

class TcpSession {
	TCPsocket _socket;
	Uint32 _lastTime;

public:
	TcpSession(TCPsocket socket, Uint32 lastTime);
	virtual ~TcpSession();

	void setLastTime(Uint32 lastTime);
	Uint32 getLastTime() const;
	TCPsocket getSocket();
};

} /* namespace jkt */

#endif /* SRC_RESEAU_TCP_TCPSESSION_H_ */
