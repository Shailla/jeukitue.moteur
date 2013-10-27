/*
 * UdpCommunicationTest.cpp
 *
 *  Created on: 26 oct. 2013
 *      Author: vgdj7997
 */

#include <iostream>
#include <string>
#include <sstream>

#include "reseau/NetworkManager.h"
#include "util/types/Bytes.h"
#include "reseau/enumReseau.h"
#include "reseau/Client.h"

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

	SDL_Delay(1000);	// Laisse démarrer le programme avant de commencer le test


	/* ********************************************************
	 * Monte un serveur
	 * ********************************************************/

	log("Monte un serveur");

	JktNet::NetworkManager* serverNM = new NetworkManager();
	NotConnectedInterlocutor2* serverInterlocutor = serverNM->ouvreServer(serverPort, serverTreePort);

	ASSERT_NOT_NULL(serverInterlocutor, "");

	/* ********************************************************
	 * Monte un client
	 * ********************************************************/

	log("Monte un client");

	// Monte un client vers ce serveur
	JktNet::NetworkManager* clientNM = new NetworkManager();
	Interlocutor2* clientInterlocutor = clientNM->ouvreClient("127.0.0.1", serverPort, serverTreePort);

	ASSERT_NOT_NULL(clientInterlocutor, "");


	/* ********************************************************
	 * Teste la connexion du client au serveur
	 * ********************************************************/
	Interlocutor2* clientOfServer;

	{
		log("Calcule du temps de connexion au serveur");

		Uint32 tempsAvantConnexion = SDL_GetTicks();

		TechnicalInterlocutor::CONNEXION_STATUS status;

		for(int i = 0 ; i < 10000 ; i++) {
			status = clientNM->getClient()->getClientUdpInterlocutor()->getConnexionStatus();

			if(status == TechnicalInterlocutor::CONNEXION_STATUS::CONNECTED) {
				break;
			}

			SDL_Delay(1);
		}

		ASSERT_EQUAL(TechnicalInterlocutor::CONNEXION_STATUS::CONNECTED, status, "La connexion a echoue");

		Uint32 temps = SDL_GetTicks() - tempsAvantConnexion;

		stringstream message;
		message << "Temps de connexion au serveur : " << temps << " ms";
		log(message);

		ASSERT_TRUE(temps < 5, "Le temps de connexion est trop long");

		clientOfServer = serverInterlocutor->popNewInterlocutor();

		ASSERT_NOT_NULL(clientOfServer, "La connexion est erronee");
	}


	/* ********************************************************
	 * Teste l'envoie d'un message du client vers le serveur
	 * ********************************************************/
	{
		log("Calcul du temps d'envoie d'un message du client vers le serveur");

		clientInterlocutor->pushDataToSend(new Bytes(data1));

		Bytes* data;
		Uint32 tempsAvantEnvoi = SDL_GetTicks();

		for(int i = 0 ; i < 10000 ; i++) {
			data = clientOfServer->popDataReceived();

			if(data) {
				break;
			}

			SDL_Delay(1);
		}

		Uint32 temps = SDL_GetTicks() - tempsAvantEnvoi;

		stringstream message;
		message << "Temps d'envoie d'un message du client vers le serveur : " << temps << " ms";
		log(message);

		ASSERT_NOT_NULL(data, "Aucune donnee recue malgre un long temps d'attente");
		ASSERT_EQUAL(data1, data->getBytes(), "La donnee recue ne correspond pas a la donnee envoyee");
		ASSERT_TRUE(temps < 5, "Le temps d'envoi d'un message du client vers le serveur est trop long");
	}


	/* ********************************************************
	 * Teste l'envoie d'un message du serveur vers le client
	 * ********************************************************/
	{
		log("Calcul du temps d'envoie d'un message du serveur vers le client");

		clientOfServer->pushDataToSend(new Bytes(data2));

		Uint32 tempsAvantEnvoi = SDL_GetTicks();
		Bytes* data;

		for(int i = 0 ; i < 10000 ; i++) {
			data = clientInterlocutor->popDataReceived();

			if(data) {
				break;
			}

			SDL_Delay(1);
		}

		Uint32 temps = SDL_GetTicks() - tempsAvantEnvoi;

		stringstream message;
		message << "Temps d'envoie d'un message du serveur vers le client : " << temps << " ms";
		log(message);

		ASSERT_NOT_NULL(data, "Aucune donnee recue malgre un long temps d'attente");
		ASSERT_EQUAL(data2, data->getBytes(), "La donnee recue ne correspond pas a la donnee envoyee");
		ASSERT_TRUE(temps < 5, "Le temps d'envoi d'un message du serveur vers le client est trop long");

	}
}


} // JktTest
