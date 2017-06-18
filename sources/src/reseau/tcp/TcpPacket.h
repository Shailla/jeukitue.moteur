/*
 * TcpPacket.h
 *
 *  Created on: 18 juin 2017
 *      Author: Erwin
 */

#ifndef SRC_RESEAU_TCP_TCPPACKET_H_
#define SRC_RESEAU_TCP_TCPPACKET_H_

#include <string>

#include "reseau/tcp/TcpSession.h"

using namespace std;

namespace jkt {

class TcpPacket {
	std::string _data;
	TcpSession* _session;
public:
	TcpPacket(TcpSession* session, char* data, int size);
	virtual ~TcpPacket();

	const std::string& getData() const;
	TcpSession* getSession() const;
};

} /* namespace jkt */

#endif /* SRC_RESEAU_TCP_TCPPACKET_H_ */
