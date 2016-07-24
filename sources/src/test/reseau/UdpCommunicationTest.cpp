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
using namespace jkt;
using namespace jkt;

namespace jkt {

UdpCommunicationTest::UdpCommunicationTest() : Test("UdpCommunicationTest") {
}

UdpCommunicationTest::~UdpCommunicationTest() {
}

string UdpCommunicationTest::getDescription() {
	string description;

	description += "Tests réalisés :";
	description += "\n - Crée un client et un serveur UDP";
	description += "\n - Connecte le client au serveur";
	description += "\n - Envoie des données du client au serveur";
	description += "\n - Envoie des données du serveur au client";

	return description;
}

void UdpCommunicationTest::test() {

	/* ********************************************************
	 * Test data
	 * ********************************************************/

	Uint16 serverPort = 777;
	Uint16 serverTreePort = 778;

	const string data1 = "Coucou";
	const string data2 = "Hello";

	SDL_Delay(1000);	// Laisse démarrer le programme avant de commencer le test


	/* ********************************************************
	 * Monte un serveur
	 * ********************************************************/

	log("Monte un serveur", __LINE__);

	jkt::NetworkManager serverNM;
	NotConnectedInterlocutor2* serverInterlocutor = serverNM.ouvreServer(serverPort, serverTreePort);

	ASSERT_NOT_NULL(serverInterlocutor, "");

	/* ********************************************************
	 * Monte un client
	 * ********************************************************/

	log("Monte un client", __LINE__);

	// Monte un client vers ce serveur
	jkt::NetworkManager* clientNM = new NetworkManager();
	Interlocutor2* clientInterlocutor = clientNM->ouvreClient("127.0.0.1", serverPort, serverTreePort);

	ASSERT_NOT_NULL(clientInterlocutor, "");


	/* ********************************************************
	 * Teste la connexion du client au serveur
	 * ********************************************************/
	Interlocutor2* clientOfServer;

	{
		log("Calcul du temps de connexion au serveur", __LINE__);

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

		ostringstream message;
		message << "Temps de connexion au serveur : " << temps << " ms";
		log(message, __LINE__);

		if(temps > 10) {
			log("WARNING : Le temps d'exécution devrait être inférieur à 10ms, si vous exécutez avec DrMemory par contre ça peut expliquer d'être au-delà de 10ms", __LINE__);
		}

		ASSERT_TRUE(temps < 300, "Le temps de connexion est trop long");	// Valeur élevée car en exécution avec DrMemory sinon ça échoue, mais le résultat devrait toujours être inférieur à 10ms
		clientOfServer = serverInterlocutor->popNewInterlocutor();
		ASSERT_NOT_NULL(clientOfServer, "La connexion est erronee");
	}


	/* ********************************************************
	 * Teste l'envoie d'un message du client vers le serveur
	 * ********************************************************/
	{
		log("Calcul du temps d'envoie d'un message du client vers le serveur", __LINE__);

		clientInterlocutor->pushDataToSend(new Bytes(data1));

		Bytes* data;
		Uint32 tempsAvantEnvoi = SDL_GetTicks();

		int maxTimeMs = 10000;
		int i;

		for(i = 0 ; i < maxTimeMs ; i++) {
			data = clientOfServer->popDataReceived();

			if(data) {
				break;
			}

			SDL_Delay(1);
		}

		ASSERT_TRUE(i < maxTimeMs, "Temps d'attente max du message atteint");

		Uint32 temps = SDL_GetTicks() - tempsAvantEnvoi;

		ostringstream message;
		message << "Temps d'envoie d'un message du client vers le serveur : " << temps << " ms";
		log(message, __LINE__);

		ASSERT_NOT_NULL(data, "Aucune donnee recue malgre un long temps d'attente");
		ASSERT_EQUAL(data1, string(data->getBytes(), data->size()), "La donnee recue ne correspond pas a la donnee envoyee");
		ASSERT_TRUE(temps < 10, "Le temps d'envoi d'un message du client vers le serveur est trop long");
	}


	/* ********************************************************
	 * Teste l'envoie d'un message du serveur vers le client
	 * ********************************************************/
	{
		log("Calcul du temps d'envoie d'un message du serveur vers le client", __LINE__);

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

		ostringstream message;
		message << "Temps d'envoie d'un message du serveur vers le client : " << temps << " ms";
		log(message, __LINE__);

		ASSERT_NOT_NULL(data, "Aucune donnee recue malgre un long temps d'attente");
		ASSERT_EQUAL(data2, string(data->getBytes(), data->size()), "La donnee recue ne correspond pas a la donnee envoyee");
		ASSERT_TRUE(temps < 5, "Le temps d'envoi d'un message du serveur vers le client est trop long");
	}
}


} // JktTest
