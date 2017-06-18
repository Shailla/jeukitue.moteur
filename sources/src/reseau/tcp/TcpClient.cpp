/*
 * TcpClient.cpp
 *
 *  Created on: 16 juin 2017
 *      Author: Erwin
 */

#include <reseau/tcp/TcpClient.h>

namespace jkt {

TcpClient::TcpClient(TCPsocket socket, Uint32 lastTime) {
	_socket = socket;
	_lastTime = lastTime;
}

TcpClient::~TcpClient() {
}

void TcpClient::setLastTime(Uint32 lastTime) {
	_lastTime = lastTime;
}

Uint32 TcpClient::getLastTime() const {
	return _lastTime;
}

TCPsocket TcpClient::getSocket() {
	return _socket;
}

} /* namespace jkt */
