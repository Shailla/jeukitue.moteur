/*
 * DataTreeTest.cpp
 *
 *  Created on: 29 oct. 2013
 *      Author: Erwin
 */

#include <iostream>

using namespace std;

#include "data/ValeurString.h"
#include "data/ValeurFloat.h"
#include "data/ValeurInt.h"
#include "reseau/new/Interlocutor2.h"
#include "data/MarqueurDistant.h"
#include "data/ServeurDataTree.h"
#include "data/ClientDataTree.h"

#include "test/data/DataTreeTest.h"

using namespace JktUtils;

namespace JktTest {

DataTreeTest::DataTreeTest() :
		Test("DataTreeTest"),
		interlocutorClient0(SDL_CreateCond(), SDL_CreateMutex()),
		interlocutorClient1(SDL_CreateCond(), SDL_CreateMutex()),
		serverTree(),
		client0Tree("client-0", &interlocutorClient0),
		client1Tree("client-1", &interlocutorClient1) {
}

DataTreeTest::~DataTreeTest() {
}

string DataTreeTest::getDescription() {
	string description;

	description += "Tests réalisés :";
	description += "\n - Creation et utilisation arbre de données serveur";
	description += "\n - Connexion et échange des données entre serveur et client";
	description += "\n - Echanges de données multi-clients";

	description += "\n\nAu long du test :";
	description += "\n - Vérifie que les données et leurs caractéristiques (révision, identifiants, marqueurs, ...) sont correctement initialisées";
	description += "\n - Vérifie que les échanges client(s)/serveur s'opèrent et se stabilisent correctement";

	return description;
}

void DataTreeTest::test() {
	initTestData();			// Initialise les données utilisées par le test
	serverTests();			// Initialise et peuple un arbre serveur, vérifie s'il est bien initialisé et peuplé
	clientTests();			// Initialise et synchronise sur le serveur un arbre client, vérifie s'il est bien initialisé et synchronisé
	multiClientsTests();	// Teste la diffusion de données sur plusieurs clients
}

void DataTreeTest::initTestData() {
	log("CREATION DES DONNEES DE TEST", __LINE__);

	// Données serveur
	branche0ServerFullId.push_back(branche0ServerId);

	valeurStringServerFullId = branche0ServerFullId;
	valeurStringServerFullId.push_back(valeurStringServerId);

	valeurIntServerFullId = branche0ServerFullId;
	valeurIntServerFullId.push_back(valeurIntServerId);

	valeurFloatServerFullId = branche0ServerFullId;
	valeurFloatServerFullId.push_back(valeurFloatServerId);

	// Données client
	branche0Client0FullId.push_back(branche0Client0Id);

	valeur0Client0FullId.push_back(valeur0Client0Id);

	branche1Client0FullId = branche0Client0FullId;
	branche1Client0FullId.push_back(branche1Client0Id);

	valeur1Client0FullId = branche1Client0FullId;
	valeur1Client0FullId.push_back(valeur1Client0Id);

	branche2Client0FullId = branche0Client0FullId;
	branche2Client0FullId.push_back(branche2Client0Id);

	valeur2Client0FullId = branche2Client0FullId;
	valeur2Client0FullId.push_back(valeur2Client0Id);

	valeur3Client0FullId = branche2Client0FullId;
	valeur3Client0FullId.push_back(valeur3Client0Id);
}

void DataTreeTest::serverTests() {

	/* ****************************************************************************
	 * Serveur : Création du serveur
	 * ***************************************************************************/

	log("CREATION DU SERVEUR", __LINE__);

	// Vérification de la branche root
	Branche& serveurRoot = serverTree.getRoot();
	ASSERT_EQUAL(rootName, serveurRoot.getBrancheName(), "Le nom de la branche root est incorrect");
	ASSERT_EQUAL(0, serveurRoot.getRevision(), "La révision initiale d'une branche devrait être nulle");


	/* ****************************************************************************
	 * Serveur : Test d'ajout d'une branche
	 * ***************************************************************************/

	log("CREATION DE DONNEES SUR LE SERVEUR", __LINE__);

	{
		Branche* branche0Tmp = serverTree.createBranche(rootFullId, branche0ServerName);
		Branche* branche0 = serverTree.getBranche(branche0ServerFullId);

		ASSERT_EQUAL(branche0Tmp, branche0, "La branche créée et celle lue devraient être les mêmes");
		ASSERT_EQUAL(branche0ServerName, branche0->getBrancheName(), "Le nom de la branche 0 est incorrect");
		ASSERT_EQUAL(0, branche0->getRevision(), "La révision initiale d'une branche devrait être nulle");
	}


	/* ****************************************************************************
	 * Serveur : Test d'ajout de valeurs
	 * ***************************************************************************/

	// Valeur string
	{
		valeurServeurString = (ValeurString*)serverTree.createValeur(branche0ServerFullId, valeurStringServerName, AnyData(valeurStringServerValue));
		ValeurString* valeurString = (ValeurString*)serverTree.getValeur(branche0ServerFullId, valeurStringServerId);

		ASSERT_EQUAL(valeurServeurString, valeurString, "La valeur créée et celle lue devraient être les mêmes");
		ASSERT_EQUAL(valeurStringServerName, valeurString->getValeurName(), "Le nom de la valeur string est incorrect");
		ASSERT_EQUAL(0, valeurString->getRevision(), "La révision initiale d'une valeur devrait être nulle");
		ASSERT_EQUAL(valeurStringServerValue, valeurString->getValeur(), "La valeur de la valeur est fausse");
	}

	// Valeur int
	{
		valeurServeurInt = (ValeurInt*)serverTree.createValeur(branche0ServerFullId, valeurIntServerName, AnyData(valeurIntServerValue));
		ValeurInt* valeurInt = (ValeurInt*)serverTree.getValeur(branche0ServerFullId, valeurIntServerId);

		ASSERT_EQUAL(valeurServeurInt, valeurInt, "La valeur créée et celle lue devraient être les mêmes");
		ASSERT_EQUAL(valeurIntServerName, valeurInt->getValeurName(), "Le nom de la valeur string est incorrect");
		ASSERT_EQUAL(0, valeurInt->getRevision(), "La révision initiale d'une valeur devrait être nulle");
		ASSERT_EQUAL(valeurIntServerValue, valeurInt->getValeur(), "La valeur de la valeur est fausse");
	}

	// Valeur float
	{
		valeurServeurFloat = (ValeurFloat*)serverTree.createValeur(branche0ServerFullId, valeurFloatServerName, AnyData(valeurFloatServerValue));
		ValeurFloat* valeurFloat = (ValeurFloat*) serverTree.getValeur(branche0ServerFullId, valeurFloatServerId);

		ASSERT_EQUAL(valeurServeurFloat, valeurFloat, "La valeur créée et celle lue devraient être les mêmes");
		ASSERT_EQUAL(valeurFloatServerName, valeurFloat->getValeurName(), "Le nom de la valeur string est incorrect");
		ASSERT_EQUAL(0, valeurFloat->getRevision(), "La révision initiale d'une valeur devrait être nulle");
		ASSERT_EQUAL(valeurFloatServerValue, valeurFloat->getValeur(), "La valeur de la valeur est fausse");
	}

	this->serveurRoot = &serveurRoot;
}

void DataTreeTest::clientTests() {

	/* ****************************************************************************
	 * Client : Création d'un client (client-0) connecté au serveur
	 * ***************************************************************************/

	log("CREATION D'UN CLIENT CONNECTE - client 0", __LINE__);
	interlocutorClient0.setName("proxyClient0");

	// Vérification de la branche root
	Branche& root = client0Tree.getRoot();
	ASSERT_EQUAL(rootName, root.getBrancheName(), "Le nom de la branche root du client est incorrect");
	ASSERT_EQUAL(0, root.getRevision(), "La révision initiale d'une branche devrait être nulle");

	// Connexion à l'arbre serveur
	serverTree.addDistant(&interlocutorClient0);

	// Vérifie le nombre de marqueurs créés pour le client (nombre de marqueurs = 1 par branche et 1 par valeur)
	const std::vector<DistantTreeProxy*> distants = serverTree.getDistants();
	DistantTreeProxy* distant = distants.at(0);
	std::map<Donnee*, MarqueurDistant*> serveurMarqueurs = distant->getMarqueurs();
	ASSERT_EQUAL(5, serveurMarqueurs.size(), "Le distant du client n'a pas le bon nombre de marqueurs");

	/* ****************************************************************************
	 * Serveur : Vérifie si les marqueurs des données du client sont bien
	 * initialisés
	 * ***************************************************************************/

	std::map<Donnee*, MarqueurDistant*>::iterator marqueurIt;

	for(marqueurIt = serveurMarqueurs.begin(); marqueurIt != serveurMarqueurs.end() ; marqueurIt++) {
		Donnee* donnee = marqueurIt->first;
		MarqueurDistant* marqueur = marqueurIt->second;

		// Root n'a pas besoin d'être envoyé au client car il est créé par défaut donc ses marqueurs sont initialisés à 0 au lieu de -1 en révisions
		if(donnee == serveurRoot) {
			ASSERT_EQUAL(0, marqueur->getTemporaryId(), "L'identifiant temporaire est mal initialisé");
			ASSERT_EQUAL(0, marqueur->getSentRevision(), "La révision envoyée est mal initialisée");
			ASSERT_EQUAL(0, marqueur->getConfirmedRevision(), "La révision confirmée est mal initialisée");
		}
		else {
			ASSERT_EQUAL(0, marqueur->getTemporaryId(), "L'identifiant temporaire est mal initialisé");
			ASSERT_EQUAL(MarqueurDistant::MARQUEUR_REVISION_INIT, marqueur->getSentRevision(), "La révision envoyée est mal initialisée");
			ASSERT_EQUAL(MarqueurDistant::MARQUEUR_REVISION_INIT, marqueur->getConfirmedRevision(), "La révision confirmée est mal initialisée");
		}
	}


	/* ****************************************************************************
	 * Client : Synchronisation et échange des données du serveur vers le client
	 * => Le serveur envoie les changements vers le client
	 * ***************************************************************************/

	log("DIFFUSION DES DONNEES DU SERVEUR VERS CLIENT-0", __LINE__);

	serverTree.diffuseChangementsToClients();
	echangeDonneesClientServeur(__LINE__, interlocutorClient0);
	client0Tree.receiveChangementsFromServer();

	// Vérifie que le serveur et client-1 ont exactement le même arbre
	ASSERT_EQUAL(serverTree.getRoot().print(0, false), client0Tree.getRoot().print(0, false), "Les abres client-0 et serveur devraient être identiques");


	/* ****************************************************************************
	 * Serveur : Vérifie si les marqueurs des données du client sont bien mis
	 * à jour suite à l'envoi des données
	 * ***************************************************************************/

	for(marqueurIt = serveurMarqueurs.begin(); marqueurIt != serveurMarqueurs.end() ; marqueurIt++) {
		Donnee* donnee = marqueurIt->first;
		MarqueurDistant* marqueur = marqueurIt->second;

		// Root n'a pas besoin d'être envoyé au client car il est créé par défaut donc ses marqueurs sont initialisés à 0 au lieu de -1 en révisions
		if(donnee == serveurRoot) {
			ASSERT_EQUAL(0, marqueur->getTemporaryId(), "L'identifiant temporaire est mal initialisé");
			ASSERT_EQUAL(0, marqueur->getSentRevision(), "La révision envoyée est mal initialisée");
			ASSERT_EQUAL(0, marqueur->getConfirmedRevision(), "La révision confirmée est mal initialisée");
		}
		else {
			ASSERT_EQUAL(0, marqueur->getTemporaryId(), "L'identifiant temporaire est mal initialisé");
			ASSERT_EQUAL(0, marqueur->getSentRevision(), "La révision envoyée est fausse");
			ASSERT_EQUAL(MarqueurDistant::MARQUEUR_REVISION_INIT, marqueur->getConfirmedRevision(), "La révision confirmée ne devrait pas avoir changé");
		}
	}


	/* ****************************************************************************
	 * Client : Vérifie la branche 0
	 * ***************************************************************************/

	{
		Branche* branche0 = client0Tree.getBranche(branche0ServerFullId);

		ASSERT_EQUAL(branche0ServerName, branche0->getBrancheName(), "Le nom de la branche 0 est incorrect");
		ASSERT_EQUAL(0, branche0->getRevision(), "La révision initiale d'une branche devrait être nulle");
	}


	/* ****************************************************************************
	 * Client 0 : Vérifie les données reçues provenant de l'arbre du serveur
	 * ***************************************************************************/

	// Valeur string
	{
		ValeurString* valeurString = (ValeurString*)client0Tree.getValeur(branche0ServerFullId, valeurStringServerId);

		ASSERT_EQUAL(valeurStringServerName, valeurString->getValeurName(), "Le nom de la valeur string est incorrect");
		ASSERT_EQUAL(0, valeurString->getRevision(), "La révision initiale d'une valeur devrait être nulle");
		ASSERT_EQUAL(valeurStringServerValue, valeurString->getValeur(), "La valeur de la valeur est fausse");
	}

	// Valeur int
	{
		ValeurInt* valeurInt = (ValeurInt*)client0Tree.getValeur(branche0ServerFullId, valeurIntServerId);

		ASSERT_EQUAL(valeurIntServerName, valeurInt->getValeurName(), "Le nom de la valeur string est incorrect");
		ASSERT_EQUAL(0, valeurInt->getRevision(), "La révision initiale d'une valeur devrait être nulle");
		ASSERT_EQUAL(valeurIntServerValue, valeurInt->getValeur(), "La valeur de la valeur est fausse");
	}

	// Valeur float
	{
		ValeurFloat* valeurFloat = (ValeurFloat*)client0Tree.getValeur(branche0ServerFullId, valeurFloatServerId);

		ASSERT_EQUAL(valeurFloatServerName, valeurFloat->getValeurName(), "Le nom de la valeur string est incorrect");
		ASSERT_EQUAL(0, valeurFloat->getRevision(), "La révision initiale d'une valeur devrait être nulle");
		ASSERT_EQUAL(valeurFloatServerValue, valeurFloat->getValeur(), "La valeur de la valeur est fausse");
	}


	echangeDonneesClientServeur(__LINE__, interlocutorClient0);

	serverTree.receiveChangementsFromClients();


	/* ****************************************************************************
	 * Serveur : Vérifie si les marqueurs des données du client sont bien mis
	 * à jour suite à la réception des confirmations de changements du client
	 * ***************************************************************************/

	for(marqueurIt = serveurMarqueurs.begin(); marqueurIt != serveurMarqueurs.end() ; marqueurIt++) {
		Donnee* donnee = marqueurIt->first;
		MarqueurDistant* marqueur = marqueurIt->second;

		// Root n'a pas besoin d'être envoyé au client car il est créé par défaut donc ses marqueurs sont initialisés à 0 au lieu de -1 en révisions
		if(donnee == serveurRoot) {
			ASSERT_EQUAL(0, marqueur->getTemporaryId(), "L'identifiant temporaire est mal initialisé");
			ASSERT_EQUAL(0, marqueur->getSentRevision(), "La révision envoyée est mal initialisée");
			ASSERT_EQUAL(0, marqueur->getConfirmedRevision(), "La révision confirmée est mal initialisée");
		}
		else {
			ASSERT_EQUAL(0, marqueur->getTemporaryId(), "L'identifiant temporaire est mal initialisé");
			ASSERT_EQUAL(0, marqueur->getSentRevision(), "La révision envoyée est fausse");
			ASSERT_EQUAL(0, marqueur->getConfirmedRevision(), "La révision confirmée devrait avoir augmenté suite à la réception de la confirmation");
		}
	}

	// Vérifie si client et serveur sont synchronisés, plus aucun message ne devrait être à échanger entre eux
	checkSynchronisationClientServeur(__LINE__, client0Tree);


	/* ***************************************************************************
	 * Test de mise à jour de valeurs côté serveur
	 * **************************************************************************/

	string newValeurServeurStringValue = valeurStringServerValue + "ajout";
	valeurServeurString->updateValeur(newValeurServeurStringValue);

	int newValeurServeurIntValue = valeurIntServerValue + 99;
	valeurServeurInt->updateValeur(newValeurServeurIntValue);

	float newValeurServeurFloatValue = valeurFloatServerValue + 0.29f;
	valeurServeurFloat->updateValeur(newValeurServeurFloatValue);

	log("Envoi du message d'update par le serveur", __LINE__);
	serverTree.diffuseChangementsToClients();
	echangeDonneesClientServeur(__LINE__, interlocutorClient0);
	client0Tree.receiveChangementsFromServer();

	// Vérifie si les valeurs ont bien été mises à jour côté client 0
	{
		ValeurString* valeurString = (ValeurString*)client0Tree.getValeur(branche0ServerFullId, valeurStringServerId);
		cout << endl << "-->" << valeurString->getValeurName();
		ASSERT_EQUAL(1, valeurString->getRevision(), "La révision aurait du être incrémentée");
		ASSERT_EQUAL(newValeurServeurStringValue, valeurString->getValeur(), "La valeur de la valeur est fausse ou  n'a pas été mise à jour sur le client 0");

		ValeurInt* valeurInt = (ValeurInt*)client0Tree.getValeur(branche0ServerFullId, valeurIntServerId);
		ASSERT_EQUAL(1, valeurInt->getRevision(), "La révision aurait du être incrémentée");
		ASSERT_EQUAL(newValeurServeurIntValue, valeurInt->getValeur(), "La valeur de la valeur est fausse ou  n'a pas été mise à jour sur le client 0");

		ValeurFloat* valeurFloat = (ValeurFloat*)client0Tree.getValeur(branche0ServerFullId, valeurFloatServerId);
		ASSERT_EQUAL(1, valeurFloat->getRevision(), "La révision aurait du être incrémentée");
		ASSERT_EQUAL(newValeurServeurFloatValue, valeurFloat->getValeur(), "La valeur de la valeur est fausse ou  n'a pas été mise à jour sur le client 0");
	}

	log("Envoi de la confirmation par le client", __LINE__);
	client0Tree.diffuseChangementsToServer();
	echangeDonneesClientServeur(__LINE__, interlocutorClient0);
	serverTree.receiveChangementsFromClients();

	// Vérifie si client et serveur sont synchronisés, plus aucun message ne devrait être à échanger entre eux
	checkSynchronisationClientServeur(__LINE__, client0Tree);

	// Vérifie que le serveur et client-1 ont exactement le même arbre
	ASSERT_EQUAL(serverTree.getRoot().print(0, false), client0Tree.getRoot().print(0, false), "Les abres client-0 et serveur devraient être identiques");


	/* ****************************************************************************
	 * Client : Ajout de données côté client dans client 0
	 * ***************************************************************************/

	log("CREATION DE DONNEES SUR LE CLIENT", __LINE__);

	// Ajout branche 0
	Branche* branche0Client0 = client0Tree.createBranche(rootFullId, branche0Client0Name);

	// Ajout valeur 0
	ValeurInt* valeur0Client0 = (ValeurInt*)client0Tree.createValeur(branche0Client0->getBrancheFullIdOrTmpId(), valeur0Client0Name, AnyData(valeur0Client0Value));

	// Ajout branche 1
	Branche* branche1Client0 = client0Tree.createBranche(branche0Client0->getBrancheFullIdOrTmpId(), branche1Client0Name);

	// Ajout valeur 1
	ValeurInt* valeur1Client0 = (ValeurInt*)client0Tree.createValeur(branche1Client0->getBrancheFullIdOrTmpId(), valeur1Client0Name, AnyData(valeur1Client0Value));

	// Ajout branche 2
	Branche* branche2Client0 = client0Tree.createBranche(branche0Client0->getBrancheFullIdOrTmpId(), branche2Client0Name);

	// Ajout valeur 2
	ValeurInt* valeur2Client0 = (ValeurInt*)client0Tree.createValeur(branche2Client0->getBrancheFullIdOrTmpId(), valeur2Client0Name, AnyData(valeur2Client0Value));

	// Ajout valeur 3
	ValeurInt* valeur3Client0 = (ValeurInt*)client0Tree.createValeur(branche2Client0->getBrancheFullIdOrTmpId(), valeur3Client0Name, AnyData(valeur3Client0Value));

	{
		ostringstream arbre;
		arbre << endl << "ARBRE CLIENT 0 :";
		client0Tree.getRoot().print(arbre, true, 0);
		log(arbre, __LINE__);
	}

	/* ****************************************************************************
	 * Vérifie si les identifiants sont bien initialisés
	 * ***************************************************************************/

	ASSERT_EQUAL(-1, branche0Client0->getBrancheId(), "Identifiant mal initialisé");
	ASSERT_EQUAL(-1, branche0Client0->getBrancheTmpId(), "Identifiant temporaire mal initialisé");

	ASSERT_EQUAL(-1, valeur0Client0->getValeurId(), "Identifiant mal initialisé");
	ASSERT_EQUAL(-1, valeur0Client0->getValeurTmpId(), "Identifiant temporaire mal initialisé");

	ASSERT_EQUAL(-1, branche1Client0->getBrancheId(), "Identifiant mal initialisé");
	ASSERT_EQUAL(-1, branche1Client0->getBrancheTmpId(), "Identifiant temporaire mal initialisé");

	ASSERT_EQUAL(-1, valeur1Client0->getValeurId(), "Identifiant mal initialisé");
	ASSERT_EQUAL(-1, valeur1Client0->getValeurTmpId(), "Identifiant temporaire mal initialisé");

	ASSERT_EQUAL(-1, branche2Client0->getBrancheId(), "Identifiant mal initialisé");
	ASSERT_EQUAL(-2, branche2Client0->getBrancheTmpId(), "Identifiant temporaire mal initialisé");

	ASSERT_EQUAL(-1, valeur2Client0->getValeurId(), "Identifiant mal initialisé");
	ASSERT_EQUAL(-1, valeur2Client0->getValeurTmpId(), "Identifiant temporaire mal initialisé");

	ASSERT_EQUAL(-1, valeur3Client0->getValeurId(), "Identifiant mal initialisé");
	ASSERT_EQUAL(-2, valeur3Client0->getValeurTmpId(), "Identifiant temporaire mal initialisé");


	/* ****************************************************************************
	 * Serveur : Synchronisation et échange des données du client vers le serveur
	 * => Le client envoie les changements vers le serveur
	 * ***************************************************************************/

	log("DIFFUSION DES DONNEES DU CLIENT", __LINE__);

	client0Tree.diffuseChangementsToServer();
	echangeDonneesClientServeur(__LINE__, interlocutorClient0);
	serverTree.receiveChangementsFromClients();

	{
		ostringstream arbre;
		arbre << "ARBRE SERVER :";
		serverTree.getRoot().print(arbre, true, 0);
		log(arbre, __LINE__);
	}


	/* ****************************************************************************
	 * Serveur : Vérifie si le serveur a bien reçu les données du client
	 * ***************************************************************************/

	// Branche 0
	Branche* branche0Client0S = serverTree.getBranche(branche0Client0FullId);
	ASSERT_NOT_NULL(branche0Client0S, "Le serveur n'a pas recu la branche");
	ASSERT_EQUAL(branche0Client0Name, branche0Client0S->getBrancheName(), "La branche recue par le serveur est mal nommée");

	// Valeur 0
	ValeurInt* valeur0Client0S = (ValeurInt*)serverTree.getValeur(branche0Client0FullId, valeur0Client0Id);
	ASSERT_NOT_NULL(valeur0Client0S, "Le serveur n'a pas recu la valeur");
	ASSERT_EQUAL(valeur0Client0Name, valeur0Client0S->getValeurName(), "La valeur recue par le serveur est mal nommée");
	ASSERT_EQUAL(valeur0Client0Value, valeur0Client0S->getValeur(), "La valeur recue par le serveur est mal nommée");

	// Branche 1
	Branche* branche1Client0S = serverTree.getBranche(branche1Client0FullId);
	ASSERT_NOT_NULL(branche1Client0S, "Le serveur n'a pas recu la branche");
	ASSERT_EQUAL(branche1Client0Name, branche1Client0S->getBrancheName(), "La branche recue par le serveur est mal nommée");

	// Valeur 1
	ValeurInt* valeur1Client0S = (ValeurInt*)serverTree.getValeur(branche1Client0FullId, valeur1Client0Id);
	ASSERT_NOT_NULL(valeur1Client0S, "Le serveur n'a pas recu la valeur");
	ASSERT_EQUAL(valeur1Client0Name, valeur1Client0S->getValeurName(), "La valeur recue par le serveur est mal nommée");
	ASSERT_EQUAL(valeur1Client0Value, valeur1Client0S->getValeur(), "La valeur recue par le serveur est mal nommée");

	// Branche 2
	Branche* branche2Client0S = serverTree.getBranche(branche2Client0FullId);
	ASSERT_NOT_NULL(branche2Client0S, "Le serveur n'a pas recu la branche");
	ASSERT_EQUAL(branche2Client0Name, branche2Client0S->getBrancheName(), "La branche recue par le serveur est mal nommée");

	// Valeur 2
	ValeurInt* valeur2Client0S = (ValeurInt*)serverTree.getValeur(branche2Client0FullId, valeur2Client0Id);
	ASSERT_NOT_NULL(valeur2Client0S, "Le serveur n'a pas recu la valeur");
	ASSERT_EQUAL(valeur2Client0Name, valeur2Client0S->getValeurName(), "La valeur recue par le serveur est mal nommée");
	ASSERT_EQUAL(valeur2Client0Value, valeur2Client0S->getValeur(), "La valeur recue par le serveur est mal nommée");

	// Valeur 3
	ValeurInt* valeur3Client0S = (ValeurInt*)serverTree.getValeur(branche2Client0FullId, valeur3Client0Id);
	ASSERT_NOT_NULL(valeur3Client0S, "Le serveur n'a pas recu la valeur");
	ASSERT_EQUAL(valeur3Client0Name, valeur3Client0S->getValeurName(), "La valeur recue par le serveur est mal nommée");
	ASSERT_EQUAL(valeur3Client0Value, valeur3Client0S->getValeur(), "La valeur recue par le serveur est mal nommée");


	/* *******************************************************************
	 * Transmission au client 0 des acceptations de changements
	 * émises par le serveur.
	 * Elles ont pour rôle de provoquer l'arrêt de l'envoi des changements
	 * par le client lorsqu'il comprend que le serveur les a bien pris
	 * en compte.
	 * ******************************************************************/

	log("DIFFUSION DES ACCEPTATIONS DU SERVEUR", __LINE__);

	ASSERT_EQUAL(0, interlocutorClient0.countDataReceived(), "Il ne devrait rester aucun message à recevoir");
	ASSERT_EQUAL(1, interlocutorClient0.countDataToSend(), "Un seul message contenant les acceptations aurait du avoir été envoyé par le serveur");
	ASSERT_EQUAL(0, interlocutorClient0.countTechnicalMessagesReceived(), "Il ne devrait rester aucun message technique à recevoir");
	ASSERT_EQUAL(0, interlocutorClient0.countTechnicalMessagesToSend(), "Il ne devrait rester aucun message technique à envoyer");

	echangeDonneesClientServeur(__LINE__, interlocutorClient0);

	ASSERT_EQUAL(1, interlocutorClient0.countDataReceived(), "Le message contenant les acceptations aurait du avoir été recu du client 0");
	ASSERT_EQUAL(0, interlocutorClient0.countDataToSend(), "Il ne devrait rester aucun message à envoyer");

	client0Tree.receiveChangementsFromServer();

	log("DIFFUSION DES CONFIRMATIONS DU CLIENT 0", __LINE__);

	ASSERT_EQUAL(0, interlocutorClient0.countDataReceived(), "Il ne devrait rester aucun message recu");
	ASSERT_EQUAL(1, interlocutorClient0.countDataToSend(), "Un seul message contenant les confirmations aurait du avoir été envoyé par le client 0");

	echangeDonneesClientServeur(__LINE__, interlocutorClient0);

	ASSERT_EQUAL(1, interlocutorClient0.countDataReceived(), "Le message contenant les confirmations aurait du avoir été recu du serveur");
	ASSERT_EQUAL(0, interlocutorClient0.countDataToSend(), "Il ne devrait rester aucun message à envoyer");

	serverTree.receiveChangementsFromClients();

	// Vérifie si client et serveur sont synchronisés, plus aucun message ne devrait être à échanger entre eux
	checkSynchronisationClientServeur(__LINE__, client0Tree);

	// Vérifie que le serveur et client-0 ont exactement le même arbre
	ASSERT_EQUAL(serverTree.getRoot().print(0, false), client0Tree.getRoot().print(0, false), "Les abres client-0 et serveur devraient être identiques");
}

void DataTreeTest::multiClientsTests() {
	/* ****************************************************************************
	 * Client : Création d'un second client (client-1) connecté au serveur
	 * ***************************************************************************/

	log("CREATION D'UN SECOND CLIENT CONNECTE - client 1", __LINE__);
	interlocutorClient1.setName("proxyClient1");

	// Connexion à l'arbre serveur
	serverTree.addDistant(&interlocutorClient1);

	log("DIFFUSION DES DONNEES DU SERVEUR VERS CLIENT-1", __LINE__);

	// Envoi des données de l'arbre serveur au client
	serverTree.diffuseChangementsToClients();
	echangeDonneesClientServeur(__LINE__, interlocutorClient1);
	client1Tree.receiveChangementsFromServer();

	// Traitement des confirmation que la client a bien reçu les données
	client1Tree.diffuseChangementsToServer();
	echangeDonneesClientServeur(__LINE__, interlocutorClient1);
	serverTree.receiveChangementsFromClients();

	// L'arbre doit être stabilité ici
	checkSynchronisationClientServeur(__LINE__, client1Tree);

	// Vérifie que le serveur et client-1 ont exactement le même arbre
	ASSERT_EQUAL(serverTree.getRoot().print(0, false), client1Tree.getRoot().print(0, false), "Les abres client-1 et serveur devraient être identiques");

	// Les arbres doivent désormais être stabilisés
	checkSynchronisationClientServeur(__LINE__, client0Tree);
	checkSynchronisationClientServeur(__LINE__, client1Tree);


	/* ****************************************************************************
	 * Création de données par client-1
	 * ***************************************************************************/

	log("CREATION DE DONNEES PAR CLIENT 1", __LINE__);

	client1Tree.createValeur(branche0ServerFullId, "valeur-0-client1", AnyData(41));
	client1Tree.createValeur(branche0ServerFullId, "valeur-1-client1", AnyData("Yoyo"));
	client1Tree.createValeur(branche2Client0FullId, "valeur-2-client1", AnyData(1.0235f));
	Branche* branche0Client1 = client1Tree.createBranche(branche2Client0FullId, "branche-0-client1");
	client1Tree.createValeur(branche0Client1->getBrancheFullIdOrTmpId(), "valeur-3-client1", AnyData("Hummm"));


	// La nouvelle valeur de client 1 descend vers le serveur
	client1Tree.diffuseChangementsToServer();
	echangeDonneesClientServeur(__LINE__, interlocutorClient1);
	serverTree.receiveChangementsFromClients();

	// Le serveur diffuse la valeur de client 1 vers client 0 et informe client 1 de l'identifiant définitif de la valeur
	serverTree.diffuseChangementsToClients();
	echangeDonneesClientServeur(__LINE__, interlocutorClient0);
	echangeDonneesClientServeur(__LINE__, interlocutorClient1);
	client0Tree.receiveChangementsFromServer();
	client1Tree.receiveChangementsFromServer();

	// Client 0 et client 1 confirment au serveur qu'ils ont bien reçu les informations
	client0Tree.diffuseChangementsToServer();
	client1Tree.diffuseChangementsToServer();
	echangeDonneesClientServeur(__LINE__, interlocutorClient0);
	echangeDonneesClientServeur(__LINE__, interlocutorClient1);
	serverTree.receiveChangementsFromClients();

	// Les arbres doivent désormais être stabilisés
	checkSynchronisationClientServeur(__LINE__, client0Tree);
	checkSynchronisationClientServeur(__LINE__, client1Tree);

	// Test l'égalité des 3 arbres
	ASSERT_EQUAL(client1Tree.getRoot().print(0, false), client0Tree.getRoot().print(0, false), "Les abres client-1 et serveur devraient être identiques");
	ASSERT_EQUAL(client1Tree.getRoot().print(0, false), serverTree.getRoot().print(0, false), "Les abres client-1 et client-0 devraient être identiques");

	{
		ostringstream arbre;
		arbre << "ARBRE SERVER :";
		serverTree.getRoot().print(arbre, true, 0);
		log(arbre, __LINE__);
	}



	/* ****************************************************************************
	 * Création de données par client 0 et client 1 aux mêmes endroits
	 * ***************************************************************************/

	log("CREATION DE DONNEES PAR 2 CLIENTS SIMULTANEMENT", __LINE__);

	client0Tree.createValeur(branche0ServerFullId, "valeur-10-client0", AnyData(44));
	client1Tree.createValeur(branche0ServerFullId, "valeur-11-client1", AnyData(45));

	Branche* branche12Client0 = client0Tree.createBranche(branche2Client0FullId, "branche-12-client0");
	client0Tree.createValeur(branche12Client0->getBrancheFullIdOrTmpId(), "valeur-12-client0", AnyData("Minou"));
	Branche* branche13Client1 = client1Tree.createBranche(branche2Client0FullId, "branche-13-client1");
	client1Tree.createValeur(branche13Client1->getBrancheFullIdOrTmpId(), "valeur-13-client1", AnyData("Poilu"));

	// Les nouvelles données de client 1 et  client 2 descendent vers le serveur
	log("Envoi des données par les clients", __LINE__);
	client0Tree.diffuseChangementsToServer();
	client1Tree.diffuseChangementsToServer();

	log("Réception des données par le serveur et envoi des acceptations", __LINE__);
	echangeDonneesClientServeur(__LINE__, interlocutorClient0);
	echangeDonneesClientServeur(__LINE__, interlocutorClient1);
	serverTree.receiveChangementsFromClients();

	// Diffusion des nouvelles données par le serveur
	log("Diffusion des nouvelles données par le serveur", __LINE__);
	serverTree.diffuseChangementsToClients();
	echangeDonneesClientServeur(__LINE__, interlocutorClient0);
	echangeDonneesClientServeur(__LINE__, interlocutorClient1);

	// Réception des nouvelles données et des accceptions par les clients et envoi des confirmations
	log("Réception des nouvelles données et des accceptions par les clients et envoi des confirmations", __LINE__);
	client0Tree.receiveChangementsFromServer();
	client1Tree.receiveChangementsFromServer();

	// Réception des confirmations par le serveur
	log("Réception des confirmations par le serveur", __LINE__);
	echangeDonneesClientServeur(__LINE__, interlocutorClient0);
	echangeDonneesClientServeur(__LINE__, interlocutorClient1);
	serverTree.receiveChangementsFromClients();

	// Les arbres doivent désormais être stabilisés
	checkSynchronisationClientServeur(__LINE__, client0Tree);
	checkSynchronisationClientServeur(__LINE__, client1Tree);

	// Test l'égalité des 3 arbres
	ASSERT_EQUAL(client1Tree.getRoot().print(0, false), client0Tree.getRoot().print(0, false), "Les abres client-1 et serveur devraient être identiques");
	ASSERT_EQUAL(client1Tree.getRoot().print(0, false), serverTree.getRoot().print(0, false), "Les abres client-1 et client-0 devraient être identiques");

	{
		ostringstream arbre;
		arbre << "ARBRE SERVER :";
		serverTree.getRoot().print(arbre, true, 0);
		log(arbre, __LINE__);
	}
}

void DataTreeTest::echangeDonneesClientServeur(int line, Interlocutor2& client) {
	JktUtils::Bytes* data;
	int count = 0;


	while((data = client.popDataToSend())) {
		client.pushDataReceived(data);
		++count;
	};

	ostringstream msg;
	msg << "Nombre de paquets échangés entre " << client.getName() << " et le serveur : " << count;
	log(msg, line);
}

void DataTreeTest::checkSynchronisationClientServeur(int line, ClientDataTree& clientTree) {
	log("VERIFICATION DE LA SYNCHRONISATION CLIENT SERVEUR avec " + clientTree.getClientName(), __LINE__);

	serverTree.diffuseChangementsToClients();
	assertEqual(__FILE__, line, 0, clientTree.getDistantServer()->getInterlocutor()->countDataReceived(), "countDataReceived 1: Il ne devrait rester aucun message à recevoir");
	assertEqual(__FILE__, line, 0, clientTree.getDistantServer()->getInterlocutor()->countDataToSend(), "countDataToSend 1: Il ne devrait rester aucun message à envoyer");
	assertEqual(__FILE__, line, 0, clientTree.getDistantServer()->getInterlocutor()->countTechnicalMessagesReceived(), "countTechnicalMessagesReceived 1: Il ne devrait rester aucun message technique à recevoir");
	assertEqual(__FILE__, line, 0, clientTree.getDistantServer()->getInterlocutor()->countTechnicalMessagesToSend(), "countTechnicalMessagesToSend 1: Il ne devrait rester aucun message technique à envoyer");

	serverTree.receiveChangementsFromClients();
	assertEqual(__FILE__, line, 0, clientTree.getDistantServer()->getInterlocutor()->countDataReceived(), "countDataReceived 2: Il ne devrait rester aucun message à recevoir");
	assertEqual(__FILE__, line, 0, clientTree.getDistantServer()->getInterlocutor()->countDataToSend(), "countDataToSend 2: Il ne devrait rester aucun message à envoyer");
	assertEqual(__FILE__, line, 0, clientTree.getDistantServer()->getInterlocutor()->countTechnicalMessagesReceived(), "countTechnicalMessagesReceived 2: Il ne devrait rester aucun message technique à recevoir");
	assertEqual(__FILE__, line, 0, clientTree.getDistantServer()->getInterlocutor()->countTechnicalMessagesToSend(), "countTechnicalMessagesToSend 2: Il ne devrait rester aucun message technique à envoyer");

	clientTree.diffuseChangementsToServer();
	assertEqual(__FILE__, line, 0, clientTree.getDistantServer()->getInterlocutor()->countDataReceived(), "countDataReceived 3: Il ne devrait rester aucun message à recevoir");
	assertEqual(__FILE__, line, 0, clientTree.getDistantServer()->getInterlocutor()->countDataToSend(), "countDataToSend 3: Il ne devrait rester aucun message à envoyer");
	assertEqual(__FILE__, line, 0, clientTree.getDistantServer()->getInterlocutor()->countTechnicalMessagesReceived(), "countTechnicalMessagesReceived 3: Il ne devrait rester aucun message technique à recevoir");
	assertEqual(__FILE__, line, 0, clientTree.getDistantServer()->getInterlocutor()->countTechnicalMessagesToSend(), "countTechnicalMessagesToSend 3: Il ne devrait rester aucun message technique à envoyer");

	clientTree.receiveChangementsFromServer();
	assertEqual(__FILE__, line, 0, clientTree.getDistantServer()->getInterlocutor()->countDataReceived(), "countDataReceived 4: Il ne devrait rester aucun message à recevoir");
	assertEqual(__FILE__, line, 0, clientTree.getDistantServer()->getInterlocutor()->countDataToSend(), "countDataToSend 4: Il ne devrait rester aucun message à envoyer");
	assertEqual(__FILE__, line, 0, clientTree.getDistantServer()->getInterlocutor()->countTechnicalMessagesReceived(), "countTechnicalMessagesReceived 4: Il ne devrait rester aucun message technique à recevoir");
	assertEqual(__FILE__, line, 0, clientTree.getDistantServer()->getInterlocutor()->countTechnicalMessagesToSend(), "countTechnicalMessagesToSend 4: Il ne devrait rester aucun message technique à envoyer");
}

} /* namespace JktTest */
