/*
 * TcpPacket.cpp
 *
 *  Created on: 18 juin 2017
 *      Author: Erwin
 */

#include <reseau/tcp/TcpPacket.h>

using namespace std;

namespace jkt {

TcpPacket::TcpPacket(TcpSession* session, char* data, int size) : _data(data, size) {
	_session = session;
}

TcpPacket::~TcpPacket() {
}

const string& TcpPacket::getData() const {
	return _data;
}

TcpSession* TcpPacket::getSession() const {
	return _session;
}

} /* namespace jkt */
