/*
 * TcpClient.cpp
 *
 *  Created on: 16 juin 2017
 *      Author: Erwin
 */

#include <reseau/tcp/TcpSession.h>

namespace jkt {

TcpSession::TcpSession(TCPsocket socket, Uint32 lastTime) {
	_socket = socket;
	_lastTime = lastTime;
}

TcpSession::~TcpSession() {
}

void TcpSession::setLastTime(Uint32 lastTime) {
	_lastTime = lastTime;
}

Uint32 TcpSession::getLastTime() const {
	return _lastTime;
}

TCPsocket TcpSession::getSocket() {
	return _socket;
}

} /* namespace jkt */
