/*
 * DataTreeTest.cpp
 *
 *  Created on: 29 oct. 2013
 *      Author: Erwin
 */

#include "util/types/StringData.h"
#include "util/types/IntData.h"
#include "util/types/FloatData.h"
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

DataTreeTest::DataTreeTest() : Test("DataTreeTest") {
}

DataTreeTest::~DataTreeTest() {
}

void DataTreeTest::test() {

	/* ********************************************************
	 * Test data
	 * ********************************************************/

	/* ------------------ Global data ------------------ */

	// Server branche root
	const string rootName = "root";
	vector<int> rootFullId;

	/* ------------------ Server data ------------------ */

	// Server branche 0
	int branche0ServerId = 0;
	vector<int> branche0ServerFullId;
	branche0ServerFullId.push_back(branche0ServerId);
	const string branche0ServerName = "branche-0-server";

	// Server valeur string
	const int valeurStringServerId = 0;
	vector<int> valeurStringServerFullId = branche0ServerFullId;
	valeurStringServerFullId.push_back(valeurStringServerId);
	const string valeurStringServerName = "valeur-string-server";
	const string valeurStringServerValue = "valeur-string-server-value";

	// Server valeur int
	const int valeurIntServerId = 1;
	vector<int> valeurIntServerFullId = branche0ServerFullId;
	valeurIntServerFullId.push_back(valeurIntServerId);
	const string valeurIntServerName = "valeur-int-server";
	const float valeurIntServerValue = 637;

	// Server valeur float
	const int valeurFloatServerId = 2;
	vector<int> valeurFloatServerFullId = branche0ServerFullId;
	valeurFloatServerFullId.push_back(valeurFloatServerId);
	const string valeurFloatServerName = "valeur-float-server";
	const float valeurFloatServerValue = 6.83f;

	/* ------------------ Client data ------------------ */

	// Client branche 1
	int branche0Client0Id = 1;
	vector<int> branche0Client0FullId;
	branche0Client0FullId.push_back(branche0Client0Id);
	const string branche0Client0Name = "branche-0-client-0";

	// Client valeur 1
	int valeur0Client0Id = 0;
	vector<int> valeur0Client0FullId;
	valeur0Client0FullId.push_back(valeur0Client0Id);
	const string valeur0Client0Name = "valeur-0-client-0";
	const int valeur0Client0Value = 927;


	/* ****************************************************************************
	 * Serveur : Création du serveur
	 * ***************************************************************************/

	ServeurDataTree serverTree;

	// Vérification de la branche root
	Branche& serveurRoot = serverTree.getRoot();
	ASSERT_EQUAL(rootName, serveurRoot.getBrancheName(), "Le nom de la branche root est incorrect");
	ASSERT_EQUAL(0, serveurRoot.getRevision(), "La révision initiale d'une branche devrait être nulle");


	/* ****************************************************************************
	 * Serveur : Test d'ajout d'une branche
	 * ***************************************************************************/

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
		Valeur* valeurStringTmp = serverTree.createValeur(branche0ServerFullId, valeurStringServerName, new StringData(valeurStringServerValue));
		ValeurString* valeurString = (ValeurString*)serverTree.getValeur(branche0ServerFullId, valeurStringServerId);

		ASSERT_EQUAL(valeurStringTmp, valeurString, "La valeur créée et celle lue devraient être les mêmes");
		ASSERT_EQUAL(valeurStringServerName, valeurString->getValeurName(), "Le nom de la valeur string est incorrect");
		ASSERT_EQUAL(0, valeurString->getRevision(), "La révision initiale d'une valeur devrait être nulle");
		ASSERT_EQUAL(valeurStringServerValue, valeurString->getValeur(), "La valeur de la valeur est fausse");
	}

	// Valeur int
	{
		Valeur* valeurIntTmp = serverTree.createValeur(branche0ServerFullId, valeurIntServerName, new IntData(valeurIntServerValue));
		ValeurInt* valeurInt = (ValeurInt*)serverTree.getValeur(branche0ServerFullId, valeurIntServerId);

		ASSERT_EQUAL(valeurIntTmp, valeurInt, "La valeur créée et celle lue devraient être les mêmes");
		ASSERT_EQUAL(valeurIntServerName, valeurInt->getValeurName(), "Le nom de la valeur string est incorrect");
		ASSERT_EQUAL(0, valeurInt->getRevision(), "La révision initiale d'une valeur devrait être nulle");
		ASSERT_EQUAL(valeurIntServerValue, valeurInt->getValeur(), "La valeur de la valeur est fausse");
	}

	// Valeur float
	{
		Valeur* valeurFloatTmp = serverTree.createValeur(branche0ServerFullId, valeurFloatServerName, new FloatData(valeurFloatServerValue));
		ValeurFloat* valeurFloat = (ValeurFloat*) serverTree.getValeur(branche0ServerFullId, valeurFloatServerId);

		ASSERT_EQUAL(valeurFloatTmp, valeurFloat, "La valeur créée et celle lue devraient être les mêmes");
		ASSERT_EQUAL(valeurFloatServerName, valeurFloat->getValeurName(), "Le nom de la valeur string est incorrect");
		ASSERT_EQUAL(0, valeurFloat->getRevision(), "La révision initiale d'une valeur devrait être nulle");
		ASSERT_EQUAL(valeurFloatServerValue, valeurFloat->getValeur(), "La valeur de la valeur est fausse");
	}


	/* ****************************************************************************
	 * Client : Création d'un client connecté au serveur
	 * ***************************************************************************/

	Interlocutor2 interlocutor(SDL_CreateCond(), SDL_CreateMutex());
	ClientDataTree clientTree("client-0", &interlocutor);

	// Vérification de la branche root
	Branche& root = clientTree.getRoot();
	ASSERT_EQUAL(rootName, root.getBrancheName(), "Le nom de la branche root du client est incorrect");
	ASSERT_EQUAL(0, root.getRevision(), "La révision initiale d'une branche devrait être nulle");

	// Connexion à l'arbre serveur
	serverTree.addDistant(&interlocutor);

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
		if(donnee == &serveurRoot) {
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

	serverTree.diffuseChangementsToClients();

	JktUtils::Bytes* data;

	while((data = interlocutor.popDataToSend())) {
		interlocutor.pushDataReceived(data);
	};

	clientTree.receiveChangementsFromServer();


	/* ****************************************************************************
	 * Serveur : Vérifie si les marqueurs des données du client sont bien mis
	 * à jour suite à l'envoi des données
	 * ***************************************************************************/

	for(marqueurIt = serveurMarqueurs.begin(); marqueurIt != serveurMarqueurs.end() ; marqueurIt++) {
		Donnee* donnee = marqueurIt->first;
		MarqueurDistant* marqueur = marqueurIt->second;

		// Root n'a pas besoin d'être envoyé au client car il est créé par défaut donc ses marqueurs sont initialisés à 0 au lieu de -1 en révisions
		if(donnee == &serveurRoot) {
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
		Branche* branche0 = clientTree.getBranche(branche0ServerFullId);

		ASSERT_EQUAL(branche0ServerName, branche0->getBrancheName(), "Le nom de la branche 0 est incorrect");
		ASSERT_EQUAL(0, branche0->getRevision(), "La révision initiale d'une branche devrait être nulle");
	}


	/* ****************************************************************************
	 * Client : Vérifie les données reçues provenant de l'arbre du serveur
	 * ***************************************************************************/

	// Valeur string
	{
		ValeurString* valeurString = (ValeurString*)clientTree.getValeur(branche0ServerFullId, valeurStringServerId);

		ASSERT_EQUAL(valeurStringServerName, valeurString->getValeurName(), "Le nom de la valeur string est incorrect");
		ASSERT_EQUAL(0, valeurString->getRevision(), "La révision initiale d'une valeur devrait être nulle");
		ASSERT_EQUAL(valeurStringServerValue, valeurString->getValeur(), "La valeur de la valeur est fausse");
	}

	// Valeur int
	{
		ValeurInt* valeurInt = (ValeurInt*)clientTree.getValeur(branche0ServerFullId, valeurIntServerId);

		ASSERT_EQUAL(valeurIntServerName, valeurInt->getValeurName(), "Le nom de la valeur string est incorrect");
		ASSERT_EQUAL(0, valeurInt->getRevision(), "La révision initiale d'une valeur devrait être nulle");
		ASSERT_EQUAL(valeurIntServerValue, valeurInt->getValeur(), "La valeur de la valeur est fausse");
	}

	// Valeur float
	{
		ValeurFloat* valeurFloat = (ValeurFloat*)clientTree.getValeur(branche0ServerFullId, valeurFloatServerId);

		ASSERT_EQUAL(valeurFloatServerName, valeurFloat->getValeurName(), "Le nom de la valeur string est incorrect");
		ASSERT_EQUAL(0, valeurFloat->getRevision(), "La révision initiale d'une valeur devrait être nulle");
		ASSERT_EQUAL(valeurFloatServerValue, valeurFloat->getValeur(), "La valeur de la valeur est fausse");
	}


	/* ****************************************************************************
	 * Client : Echange des données client vers serveur
	 * => Le client envoie les confirmations de changements vers le serveur
	 * ***************************************************************************/

	while((data = interlocutor.popDataToSend())) {
		interlocutor.pushDataReceived(data);
	};

	serverTree.receiveChangementsFromClients();


	/* ****************************************************************************
	 * Serveur : Vérifie si les marqueurs des données du client sont bien mis
	 * à jour suite à la réception des confirmations de changements du client
	 * ***************************************************************************/

	for(marqueurIt = serveurMarqueurs.begin(); marqueurIt != serveurMarqueurs.end() ; marqueurIt++) {
		Donnee* donnee = marqueurIt->first;
		MarqueurDistant* marqueur = marqueurIt->second;

		// Root n'a pas besoin d'être envoyé au client car il est créé par défaut donc ses marqueurs sont initialisés à 0 au lieu de -1 en révisions
		if(donnee == &serveurRoot) {
			ASSERT_EQUAL(0, marqueur->getTemporaryId(), "L'identifiant temporaire est mal initialisé");
			ASSERT_EQUAL(0, marqueur->getSentRevision(), "La révision envoyée est mal initialisée");
			ASSERT_EQUAL(0, marqueur->getConfirmedRevision(), "La révision confirmée est mal initialisée");
		}
		else {
			ASSERT_EQUAL(0, marqueur->getTemporaryId(), "L'identifiant temporaire est mal initialisé");
			ASSERT_EQUAL(0, marqueur->getSentRevision(), "La révision envoyée est fausse");
			ASSERT_EQUAL(0, marqueur->getConfirmedRevision(), "La révision confirmée ne devrait pas avoir changé");
		}
	}


	/* ****************************************************************************
	 * Client : Ajout de données côté client
	 * ***************************************************************************/

	// Ajout d'une branche
	Branche* brancheClient1 = clientTree.createBranche(rootFullId, branche0Client0Name);

	// Ajout d'une valeur
	Valeur* valeurClient1 = clientTree.createValeur(brancheClient1, valeur0Client0Name, new IntData(valeur0Client0Value));
	{
		ostringstream arbre;
		arbre << endl << "ARBRE CLIENT 0 :";
		clientTree.getRoot().print(arbre, 0);
		log(arbre);
	}

	/* ****************************************************************************
	 * Serveur : Vérifie si les marqueurs des données du client sont bien mis
	 * à jour suite à la réception des confirmations de changements du client
	 * ***************************************************************************/

	DistantTreeProxy* distantServer = clientTree.getDistantServer();

	// Vérification marqueur de branche 1
	MarqueurDistant* marqueurBrancheClient1 = distantServer->getMarqueur(brancheClient1);
	ASSERT_EQUAL(0, marqueurBrancheClient1->getTemporaryId(), "L'identifiant temporaire est mal initialisé");
	ASSERT_EQUAL(MarqueurDistant::MARQUEUR_REVISION_INIT, marqueurBrancheClient1->getSentRevision(), "La révision envoyée est mal initialisée");
	ASSERT_EQUAL(MarqueurDistant::MARQUEUR_REVISION_INIT, marqueurBrancheClient1->getConfirmedRevision(), "La révision confirmée est mal initialisée");

	// Vérification marqueur de valeur 1
	MarqueurDistant* marqueurValeurClient1 = distantServer->getMarqueur(valeurClient1);
	ASSERT_EQUAL(0, marqueurValeurClient1->getTemporaryId(), "L'identifiant temporaire est mal initialisé");
	ASSERT_EQUAL(MarqueurDistant::MARQUEUR_REVISION_INIT, marqueurValeurClient1->getSentRevision(), "La révision envoyée est mal initialisée");
	ASSERT_EQUAL(MarqueurDistant::MARQUEUR_REVISION_INIT, marqueurValeurClient1->getConfirmedRevision(), "La révision confirmée est mal initialisée");


	/* ****************************************************************************
	 * Serveur : Synchronisation et échange des données du client vers le serveur
	 * => Le client envoie les changements vers le serveur
	 * ***************************************************************************/

	clientTree.diffuseChangementsToServer();

	while((data = interlocutor.popDataToSend())) {
		interlocutor.pushDataReceived(data);
	};

	serverTree.receiveChangementsFromClients();

	{
		ostringstream arbre;
		arbre << endl << "ARBRE SERVER :";
		serverTree.getRoot().print(arbre, 0);
		log(arbre);
	}

	/* ****************************************************************************
	 * Serveur : Vérifie si le serveur a bien reçu les données du client
	 * ***************************************************************************/

	Branche* branche0Client0S = serverTree.getBranche(branche0Client0FullId);
	ASSERT_NOT_NULL(branche0Client0S, "Le serveur n'a pas recu la branche");
	ASSERT_EQUAL(branche0Client0Name, branche0Client0S->getBrancheName(), "La branche recue par le serveur est mal nommée");

	ValeurInt* valeurClient1S = (ValeurInt*)serverTree.getValeur(branche0Client0FullId, valeur0Client0Id);
	ASSERT_NOT_NULL(valeurClient1S, "Le serveur n'a pas recu la valeur");
	ASSERT_EQUAL(valeur0Client0Name, valeurClient1S->getValeurName(), "La valeur recue par le serveur est mal nommée");
	ASSERT_EQUAL(valeur0Client0Value, valeurClient1S->getValeur(), "La valeur recue par le serveur est mal nommée");
}

} /* namespace JktTest */
