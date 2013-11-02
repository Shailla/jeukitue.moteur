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

	// Data branche root
	const string rootName = "root";
	vector<int> rootFullId;

	// Data branche 0
	int branche0Id = 0;
	vector<int> branche0FullId;
	branche0FullId.push_back(branche0Id);
	const string branche0Name = "branche-0";

	// Data branche 1
	int branche1Id = 1;
	vector<int> branche1FullId;
	branche1FullId.push_back(branche1Id);
	const string branche1Name = "branche-1";

	// Data valeur string
	const int valeurStringId = 0;
	vector<int> valeurStringFullId = branche0FullId;
	valeurStringFullId.push_back(valeurStringId);
	const string valeurStringName = "valeur-string";
	const string valeurStringValue = "valeur-string-value";

	// Data valeur int
	const int valeurIntId = 1;
	vector<int> valeurIntFullId = branche0FullId;
	valeurIntFullId.push_back(valeurIntId);
	const string valeurIntName = "valeur-int";
	const float valeurIntValue = 637;

	// Data valeur float
	const int valeurFloatId = 2;
	vector<int> valeurFloatFullId = branche0FullId;
	valeurFloatFullId.push_back(valeurFloatId);
	const string valeurFloatName = "valeur-float";
	const float valeurFloatValue = 6.83f;

	const string valeurClient1Name = "valeur-int-client-1";
	const int valeurClient1Value = 927;


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
		Branche* branche0Tmp = serverTree.createBranche(rootFullId, branche0Name);
		Branche* branche0 = serverTree.getBranche(branche0FullId);

		ASSERT_EQUAL(branche0Tmp, branche0, "La branche créée et celle lue devraient être les mêmes");
		ASSERT_EQUAL(branche0Name, branche0->getBrancheName(), "Le nom de la branche 0 est incorrect");
		ASSERT_EQUAL(0, branche0->getRevision(), "La révision initiale d'une branche devrait être nulle");
	}


	/* ****************************************************************************
	 * Serveur : Test d'ajout de valeurs
	 * ***************************************************************************/

	// Valeur string
	{
		Valeur* valeurStringTmp = serverTree.createValeur(branche0FullId, valeurStringName, new StringData(valeurStringValue));
		ValeurString* valeurString = (ValeurString*)serverTree.getValeur(branche0FullId, valeurStringId);

		ASSERT_EQUAL(valeurStringTmp, valeurString, "La valeur créée et celle lue devraient être les mêmes");
		ASSERT_EQUAL(valeurStringName, valeurString->getValeurName(), "Le nom de la valeur string est incorrect");
		ASSERT_EQUAL(0, valeurString->getRevision(), "La révision initiale d'une valeur devrait être nulle");
		ASSERT_EQUAL(valeurStringValue, valeurString->getValeur(), "La valeur de la valeur est fausse");
	}

	// Valeur int
	{
		Valeur* valeurIntTmp = serverTree.createValeur(branche0FullId, valeurIntName, new IntData(valeurIntValue));
		ValeurInt* valeurInt = (ValeurInt*)serverTree.getValeur(branche0FullId, valeurIntId);

		ASSERT_EQUAL(valeurIntTmp, valeurInt, "La valeur créée et celle lue devraient être les mêmes");
		ASSERT_EQUAL(valeurIntName, valeurInt->getValeurName(), "Le nom de la valeur string est incorrect");
		ASSERT_EQUAL(0, valeurInt->getRevision(), "La révision initiale d'une valeur devrait être nulle");
		ASSERT_EQUAL(valeurIntValue, valeurInt->getValeur(), "La valeur de la valeur est fausse");
	}

	// Valeur float
	{
		Valeur* valeurFloatTmp = serverTree.createValeur(branche0FullId, valeurFloatName, new FloatData(valeurFloatValue));
		ValeurFloat* valeurFloat = (ValeurFloat*) serverTree.getValeur(branche0FullId, valeurFloatId);

		ASSERT_EQUAL(valeurFloatTmp, valeurFloat, "La valeur créée et celle lue devraient être les mêmes");
		ASSERT_EQUAL(valeurFloatName, valeurFloat->getValeurName(), "Le nom de la valeur string est incorrect");
		ASSERT_EQUAL(0, valeurFloat->getRevision(), "La révision initiale d'une valeur devrait être nulle");
		ASSERT_EQUAL(valeurFloatValue, valeurFloat->getValeur(), "La valeur de la valeur est fausse");
	}


	/* ****************************************************************************
	 * Client : Création d'un client connecté au serveur
	 * ***************************************************************************/

	Interlocutor2 interlocutor(SDL_CreateCond(), SDL_CreateMutex());
	ClientDataTree clientTree("client-test", &interlocutor);

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
		Branche* branche0 = clientTree.getBranche(branche0FullId);

		ASSERT_EQUAL(branche0Name, branche0->getBrancheName(), "Le nom de la branche 0 est incorrect");
		ASSERT_EQUAL(0, branche0->getRevision(), "La révision initiale d'une branche devrait être nulle");
	}


	/* ****************************************************************************
	 * Client : Vérifie les données reçues provenant de l'arbre du serveur
	 * ***************************************************************************/

	// Valeur string
	{
		ValeurString* valeurString = (ValeurString*)clientTree.getValeur(branche0FullId, valeurStringId);

		ASSERT_EQUAL(valeurStringName, valeurString->getValeurName(), "Le nom de la valeur string est incorrect");
		ASSERT_EQUAL(0, valeurString->getRevision(), "La révision initiale d'une valeur devrait être nulle");
		ASSERT_EQUAL(valeurStringValue, valeurString->getValeur(), "La valeur de la valeur est fausse");
	}

	// Valeur int
	{
		ValeurInt* valeurInt = (ValeurInt*)clientTree.getValeur(branche0FullId, valeurIntId);

		ASSERT_EQUAL(valeurIntName, valeurInt->getValeurName(), "Le nom de la valeur string est incorrect");
		ASSERT_EQUAL(0, valeurInt->getRevision(), "La révision initiale d'une valeur devrait être nulle");
		ASSERT_EQUAL(valeurIntValue, valeurInt->getValeur(), "La valeur de la valeur est fausse");
	}

	// Valeur float
	{
		ValeurFloat* valeurFloat = (ValeurFloat*)clientTree.getValeur(branche0FullId, valeurFloatId);

		ASSERT_EQUAL(valeurFloatName, valeurFloat->getValeurName(), "Le nom de la valeur string est incorrect");
		ASSERT_EQUAL(0, valeurFloat->getRevision(), "La révision initiale d'une valeur devrait être nulle");
		ASSERT_EQUAL(valeurFloatValue, valeurFloat->getValeur(), "La valeur de la valeur est fausse");
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
	Branche* brancheClient1 = clientTree.createBranche(rootFullId, branche1Name);

	// Ajout d'une valeur
	Valeur* valeurClient1 = clientTree.createValeur(brancheClient1, valeurClient1Name, new IntData(valeurClient1Value));
	{
		ostringstream arbre;
		arbre << endl << "ARBRE CLIENT 1 :";
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
		arbre << endl << "ARBRE SERVER 1 :";
		serverTree.getRoot().print(arbre, 0);
		log(arbre);
	}

	/* ****************************************************************************
	 * Serveur : Vérifie si le serveur a bien reçu les données du client
	 * ***************************************************************************/

	Branche* brancheClient1S = serverTree.getBranche(brancheClient1->getBrancheFullId());
	ASSERT_NOT_NULL(brancheClient1S, "Le serveur n'a pas recu la branche");
	ASSERT_EQUAL(branche1Name, brancheClient1S->getBrancheName(), "La branche recue par le serveur est mal nommée");

	ValeurInt* valeurClient1S = (ValeurInt*)serverTree.getBranche(brancheClient1->getBrancheFullId());
	ASSERT_NOT_NULL(valeurClient1S, "Le serveur n'a pas recu la valeur");
	ASSERT_EQUAL(valeurClient1Name, valeurClient1S->getValeurName(), "La valeur recue par le serveur est mal nommée");
	ASSERT_EQUAL(valeurClient1Value, valeurClient1S->getValeur(), "La valeur recue par le serveur est mal nommée");
}

} /* namespace JktTest */
