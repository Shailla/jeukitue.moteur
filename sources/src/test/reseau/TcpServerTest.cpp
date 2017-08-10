/*
 * TcpServerTest.cpp
 *
 *  Created on: 23 juin 2017
 *      Author: Erwin
 */

#include <string>
#include <sstream>
#include <vector>
#include <iostream>

#include "SDL.h"
#include "SDL_net.h"

#include "reseau/tcp/TcpPacket.h"
#include "reseau/tcp/TcpServer.h"

#include "test/reseau/TcpServerTest.h"

namespace jkt {

TcpServerTest::TcpServerTest() : Test("TcpServerTest")  {
	_client1Socket = 0;
	_client2Socket = 0;
}

TcpServerTest::~TcpServerTest() {
}

string TcpServerTest::getDescription() {
	return "Tests du serveur TCP";
}

void TcpServerTest::test() {
	vector<TcpPacket*> packets;

	// Start TCP Server
	log("Starting TCP server", __LINE__);

	TcpServer server(_serverPort);
	server.setClientsSize(3);
	server.setTimeout(10);
	server.setTcpBufferSize(7);

	server.start();

	packets = server.receive(20);

	ASSERT_EQUAL(0, (int)packets.size(), "Reception pipe should be empty");

	_client1Socket = createClient();
	_client2Socket = createClient();


	// Exchange data with 2 clients
	log("Send TCP data to client 1", __LINE__);
	SDLNet_TCP_Send(_client1Socket, "Hello1", 6);

	log("Send TCP data to client 2", __LINE__);
	SDLNet_TCP_Send(_client2Socket, "Coucou1", 7);

	SDL_Delay(100);
	log("Try to connect clients and receive data", __LINE__);
	packets = server.receive(20);

	logPackets(packets);

	ASSERT_EQUAL(2, packets.size(), "Should have received 2 data packet, since each packet size is higher than the buffer size");

	ASSERT_EQUAL("Hello1", packets[0]->getData(), "");
	ASSERT_EQUAL("Coucou1", packets[1]->getData(), "");


	// Exchange data with a single client
	log("Exchange data", __LINE__);

	log("Send TCP data to client 1", __LINE__);
	SDLNet_TCP_Send(_client1Socket, "Hello2", 6);

	log("Send TCP data to client 1", __LINE__);
	SDLNet_TCP_Send(_client1Socket, "Coucou2", 7);

	SDL_Delay(10);
	packets = server.receive(20);

	logPackets(packets);

	ASSERT_EQUAL(1, packets.size(), "Should have received 1 data packet, since each packet size is higher than the buffer size");
	ASSERT_EQUAL("Hello2C", packets[0]->getData(), "");

	packets = server.receive(20);

	logPackets(packets);

	ASSERT_EQUAL(1, packets.size(), "Should have received 1 data packet, since each packet size is higher than the buffer size");
	ASSERT_EQUAL("oucou2", packets[0]->getData(), "");
}

void TcpServerTest::logPackets(vector<TcpPacket*>& packets) {
	stringstream msg;
	msg << "Paquets reçus : " << packets.size();

	for(int i=0 ; (size_t)i< packets.size() ; i++) {
		msg << endl << "\tPaquet " << i << " : '" << packets[i]->getData() << "'";
	}

	msg << endl << "end" << flush;

	log(msg.str(), __LINE__);
}

TCPsocket TcpServerTest::createClient() {
	log("Create client", __LINE__);

	// Creation de la socket serveur
	IPaddress adresse;

	if(SDLNet_ResolveHost(&adresse, "127.0.0.1", _serverPort) == -1) {
		FAIL("SDLNet_ResolveHost error");
	}

	TCPsocket clientSocket = SDLNet_TCP_Open(&adresse);

	if(!clientSocket) {
		FAIL("SDLNet_TCP_Open error");
	}

	return clientSocket;
}

} /* namespace jkt */
