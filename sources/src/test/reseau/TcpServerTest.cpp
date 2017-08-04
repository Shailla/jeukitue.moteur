/*
 * TcpServerTest.cpp
 *
 *  Created on: 23 juin 2017
 *      Author: Erwin
 */

#include <string>
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
	server.setTcpBufferSize(6);

	server.start();

	packets = server.receive(20);

	ASSERT_EQUAL(0, (int)packets.size(), "Reception pipe should be empty");

	_client1Socket = createClient();
	_client2Socket = createClient();


	// Exchange data
	log("Exchange data", __LINE__);

	SDLNet_TCP_Send(_client1Socket, "Hello1", 6);
	SDLNet_TCP_Send(_client2Socket, "Coucou1", 7);

	SDL_Delay(10);
	packets = server.receive(20);

//	ASSERT_EQUAL(2, packets.size(), "Should have received 2 data packet, since each packet size is higger than the buffer size");
	cout << endl << "Nombre de paquets : " << packets.size();

	for(int i=0 ; i< packets.size() ; i++) {
		cout << endl << "Paquet " << i << " : '" << packets[i]->getData() << "'";
	}

	cout << endl << "end" << flush;

	ASSERT_EQUAL("Hello1", packets[0]->getData(), "");
	ASSERT_EQUAL("Coucou1", packets[1]->getData(), "");

	// Exchange data
	log("Exchange data", __LINE__);

	SDLNet_TCP_Send(_client1Socket, "Hello2", 6);
	SDLNet_TCP_Send(_client1Socket, "Coucou2", 7);

	SDL_Delay(10);
	packets = server.receive(20);

	ASSERT_EQUAL(2, packets.size(), "Should have received 2 data packet, since each packet size is higger than the buffer size");
	ASSERT_EQUAL("Hello2", packets[0]->getData(), "");
	ASSERT_EQUAL("Coucou2", packets[1]->getData(), "");
}

TCPsocket TcpServerTest::createClient() {
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
