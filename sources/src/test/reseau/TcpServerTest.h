/*
 * TcpServerTest.h
 *
 *  Created on: 23 juin 2017
 *      Author: Erwin
 */

#ifndef SRC_TEST_RESEAU_TCPSERVERTEST_H_
#define SRC_TEST_RESEAU_TCPSERVERTEST_H_

#include <string>
#include <vector>

#include "SDL.h"
#include "SDL_net.h"

#include "reseau/tcp/TcpPacket.h"

#include "test/Test.h"

namespace jkt {

class TcpServerTest : public Test {
	const int _serverPort = 17594;
	TCPsocket _client1Socket;
	TCPsocket _client2Socket;

	TCPsocket createClient();
	void logPackets(std::vector<TcpPacket*>& packets);

public:
	TcpServerTest();
	virtual ~TcpServerTest();

	std::string getDescription() override;

	void test() override;
};

} /* namespace jkt */

#endif /* SRC_TEST_RESEAU_TCPSERVERTEST_H_ */
