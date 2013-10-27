/*
 * UdpCommunicationTest.cpp
 *
 *  Created on: 26 oct. 2013
 *      Author: vgdj7997
 */

#include <iostream>
#include <string>

#include "reseau/NetworkManager.h"
#include "util/types/Bytes.h"

#include "test/reseau/UdpCommunicationTest.h"

using namespace std;
using namespace JktNet;
using namespace JktUtils;

namespace JktTest {

UdpCommunicationTest::UdpCommunicationTest() : Test("UdpCommunicationTest") {
}

UdpCommunicationTest::~UdpCommunicationTest() {
}

void UdpCommunicationTest::test() {
	// Test data
	Uint16 serverPort = 777;
	Uint16 serverTreePort = 778;

	const string data1 = "Coucou";
	const string data2 = "Hello";


	/* ********************************************************
	 * Monte un serveur
	 * ********************************************************/

	JktNet::NetworkManager* serverNM = new NetworkManager();
	NotConnectedInterlocutor2* serverInterlocutor = serverNM->ouvreServer(serverPort, serverTreePort);

	ASSERT_NOT_NULL(serverInterlocutor);

	/* ********************************************************
	 * Monte un client
	 * ********************************************************/

	// Monte un client vers ce serveur
	JktNet::NetworkManager* clientNM = new NetworkManager();
	Interlocutor2* clientInterlocutor = clientNM->ouvreClient("127.0.0.1", serverPort, serverTreePort);

	ASSERT_NOT_NULL(clientInterlocutor);


	/* ********************************************************
	 * Teste la connexion du client au serveur
	 * ********************************************************/

	SDL_Delay(3000);	// Temps pour l'établissement de la connexion du client au serveur

	Interlocutor2* clientOfServer = serverInterlocutor->popNewInterlocutor();

	ASSERT_NOT_NULL(clientOfServer);


	/* ********************************************************
	 * Teste l'envoie d'un message du client vers le serveur
	 * ********************************************************/
	{
		clientInterlocutor->pushDataToSend(new Bytes(data1));

		SDL_Delay(3000);

		Bytes* data = clientOfServer->popDataReceived();
		ASSERT_NOT_NULL(data);
		ASSERT_EQUAL(data1, data->getBytes());
	}


	/* ********************************************************
	 * Teste l'envoie d'un message du serveur vers le client
	 * ********************************************************/
	{
		clientOfServer->pushDataToSend(new Bytes(data1));

		SDL_Delay(3000);

		Bytes* data = clientInterlocutor->popDataReceived();
		ASSERT_NOT_NULL(data);
		ASSERT_EQUAL(data1, data->getBytes());
	}
}


} // JktTest
