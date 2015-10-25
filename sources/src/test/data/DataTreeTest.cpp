/*
 * DataTreeTest.cpp
 *
 *  Created on: 29 oct. 2013
 *      Author: Erwin
 */

#include <iostream>

using namespace std;

#include "util/CollectionsUtils.h"
#include "data/ValeurString.h"
#include "data/ValeurFloat.h"
#include "data/ValeurInt.h"
#include "reseau/new/Interlocutor2.h"
#include "data/MarqueurDistant.h"
#include "data/ServeurDataTree.h"
#include "data/ClientDataTree.h"
#include "data/DataTreeUtils.h"

#include "test/data/DataTreeTest.h"

using namespace JktUtils;

namespace JktTest {

DataTreeTest::DataTreeTest() :
								Test("DataTreeTest"),
								interlocutorClient0(SDL_CreateCond(), SDL_CreateMutex()),
								interlocutorClient1(SDL_CreateCond(), SDL_CreateMutex()),
								serverTree(),
								client0Tree("client-0", &interlocutorClient0),
								client1Tree("client-1", &interlocutorClient1),
								distantClient0(0),
								distantClient1(0){
}

DataTreeTest::~DataTreeTest() {
}

string DataTreeTest::getDescription() {
	string description;

	description += "Tests r�alis�s :";
	description += "\n - Creation et utilisation arbre de donn�es serveur";
	description += "\n - Connexion et �change des donn�es entre serveur et client";
	description += "\n - Echanges de donn�es multi-clients";

	description += "\n\nAu long du test :";
	description += "\n - V�rifie que les donn�es et leurs caract�ristiques (r�vision, identifiants, marqueurs, ...) sont correctement initialis�es";
	description += "\n - V�rifie que les �changes client(s)/serveur s'op�rent et se stabilisent correctement";

	return description;
}

void DataTreeTest::test() {
	initTestData();			// Initialise les donn�es utilis�es par le test

	serverTests();			// Initialise et peuple un arbre serveur, v�rifie s'il est bien initialis� et peupl�
	iteratorTest();			// Utilise l'it�rateur servant � parcours l'arbre de donn�es
	clientTests();			// Initialise et synchronise sur le serveur un arbre client, v�rifie s'il est bien initialis� et synchronis�
	multiClientsTests();	// Teste l'�change de donn�es entre plusieurs clients
	privateTreeTest();		// Teste la cr�ation d'une branche priv�e de donn�es (donn�es sp�cifiques � chaque client partag�es avec le serveur)
}

void DataTreeTest::logDataTreeElementId(const string dataTreeElementName, const vector<int>& dataTreeElementId) {
	log(dataTreeElementName + " : " + CollectionsUtils::toString(dataTreeElementId), __LINE__);
}

void DataTreeTest::initTestData() {
	log("INITIALISATION DES DONNEES DU TEST", __LINE__);

	// Donn�es serveur
	branche0ServerFullId.push_back(branche0ServerId);
	logDataTreeElementId("branche0ServerFullId", branche0ServerFullId);

	valeurStringServerFullId = branche0ServerFullId;
	valeurStringServerFullId.push_back(valeurStringServerId);
	logDataTreeElementId("valeurStringServerFullId", valeurStringServerFullId);

	valeurIntServerFullId = branche0ServerFullId;
	valeurIntServerFullId.push_back(valeurIntServerId);
	logDataTreeElementId("valeurIntServerFullId", valeurIntServerFullId);

	valeurFloatServerFullId = branche0ServerFullId;
	valeurFloatServerFullId.push_back(valeurFloatServerId);
	logDataTreeElementId("valeurFloatServerFullId", valeurFloatServerFullId);

	branche1ServerFullId.push_back(branche1ServerId);

	// Donn�es client
	branche0Client0FullId.push_back(branche0Client0Id);
	logDataTreeElementId("branche0Client0FullId", branche0Client0FullId);

	valeur0Client0FullId.push_back(valeur0Client0Id);
	logDataTreeElementId("valeur0Client0FullId", valeur0Client0FullId);

	branche1Client0FullId = branche0Client0FullId;
	branche1Client0FullId.push_back(branche1Client0Id);
	logDataTreeElementId("branche1Client0FullId", branche1Client0FullId);

	valeur1Client0FullId = branche1Client0FullId;
	valeur1Client0FullId.push_back(valeur1Client0Id);
	logDataTreeElementId("valeur1Client0FullId", valeur1Client0FullId);

	branche2Client0FullId = branche0Client0FullId;
	branche2Client0FullId.push_back(branche2Client0Id);
	logDataTreeElementId("branche2Client0FullId", branche2Client0FullId);

	valeur2Client0FullId = branche2Client0FullId;
	valeur2Client0FullId.push_back(valeur2Client0Id);
	logDataTreeElementId("valeur2Client0FullId", valeur2Client0FullId);

	valeur3Client0FullId = branche2Client0FullId;
	valeur3Client0FullId.push_back(valeur3Client0Id);
	logDataTreeElementId("valeur3Client0FullId", valeur3Client0FullId);

	valeur4Client0FullId = branche2Client0FullId;
	valeur4Client0FullId.push_back(valeur4Client0Id);
}

void DataTreeTest::serverTests() {

	/* ****************************************************************************
	 * Serveur : Cr�ation du serveur
	 * ***************************************************************************/

	log("CREATION DU SERVEUR", __LINE__);

	// V�rification de la branche root
	Branche& serveurRoot = serverTree.getRoot();
	ASSERT_EQUAL(rootName, serveurRoot.getBrancheName(), "Le nom de la branche root est incorrect");
	ASSERT_EQUAL(0, serveurRoot.getRevision(), "La r�vision initiale d'une branche devrait �tre nulle");


	/* ****************************************************************************
	 * Serveur : Test d'ajout d'une branche
	 * ***************************************************************************/

	log("CREATION DE DONNEES SUR LE SERVEUR", __LINE__);

	{
		Branche* branche0Tmp = serverTree.createBranche(0, rootFullId, branche0ServerName);
		Branche* branche0 = serverTree.getBranche(0, branche0ServerFullId);

		ASSERT_EQUAL(branche0Tmp, branche0, "La branche cr��e et celle lue devraient �tre les m�mes");
		ASSERT_EQUAL(branche0ServerName, branche0->getBrancheName(), "Le nom de la branche 0 est incorrect");
		ASSERT_EQUAL(0, branche0->getRevision(), "La r�vision initiale d'une branche devrait �tre nulle");
	}


	/* ****************************************************************************
	 * Serveur : Test d'ajout de valeurs
	 * ***************************************************************************/

	// Valeur string
	{
		valeurServeurString = (ValeurString*)serverTree.createValeur(0, ANY, branche0ServerFullId, valeurStringServerName, AnyData(valeurStringServerValue));
		ValeurString* valeurString = (ValeurString*)serverTree.getValeur(0, branche0ServerFullId, valeurStringServerId);

		ASSERT_EQUAL(valeurServeurString, valeurString, "La valeur cr��e et celle lue devraient �tre les m�mes");
		ASSERT_EQUAL(valeurStringServerName, valeurString->getValeurName(), "Le nom de la valeur string est incorrect");
		ASSERT_EQUAL(0, valeurString->getRevision(), "La r�vision initiale d'une valeur devrait �tre nulle");
		ASSERT_EQUAL(valeurStringServerValue, valeurString->getValeur(), "La valeur de la valeur est fausse");
	}

	// Valeur int
	{
		valeurServeurInt = (ValeurInt*)serverTree.createValeur(0, ANY, branche0ServerFullId, valeurIntServerName, AnyData(valeurIntServerValue));
		ValeurInt* valeurInt = (ValeurInt*)serverTree.getValeur(0, branche0ServerFullId, valeurIntServerId);

		ASSERT_EQUAL(valeurServeurInt, valeurInt, "La valeur cr��e et celle lue devraient �tre les m�mes");
		ASSERT_EQUAL(valeurIntServerName, valeurInt->getValeurName(), "Le nom de la valeur string est incorrect");
		ASSERT_EQUAL(0, valeurInt->getRevision(), "La r�vision initiale d'une valeur devrait �tre nulle");
		ASSERT_EQUAL(valeurIntServerValue, valeurInt->getValeur(), "La valeur de la valeur est fausse");
	}

	// Valeur float
	{
		valeurServeurFloat = (ValeurFloat*)serverTree.createValeur(0, ANY, branche0ServerFullId, valeurFloatServerName, AnyData(valeurFloatServerValue));
		ValeurFloat* valeurFloat = (ValeurFloat*) serverTree.getValeur(0, branche0ServerFullId, valeurFloatServerId);

		ASSERT_EQUAL(valeurServeurFloat, valeurFloat, "La valeur cr��e et celle lue devraient �tre les m�mes");
		ASSERT_EQUAL(valeurFloatServerName, valeurFloat->getValeurName(), "Le nom de la valeur string est incorrect");
		ASSERT_EQUAL(0, valeurFloat->getRevision(), "La r�vision initiale d'une valeur devrait �tre nulle");
		ASSERT_EQUAL(valeurFloatServerValue, valeurFloat->getValeur(), "La valeur de la valeur est fausse");
	}

	{
		ostringstream arbre;
		arbre << "ARBRE SERVER :";
		serverTree.getRoot().print(arbre, 0, true, 0);
		log(arbre, __LINE__);
	}

	this->serveurRoot = &serveurRoot;
}

void DataTreeTest::iteratorTest() {
	BrancheIterator it = serverTree.getRoot().begin(0);

	while(++it) {
		ostringstream msg;
		msg << endl << "==> " << CollectionsUtils::toString((*it)->getBrancheFullId()) << flush;
		log(msg.str(), __LINE__);
	}
}

void DataTreeTest::clientTests() {

	/* ****************************************************************************
	 * Client : Cr�ation d'un client (client-0) connect� au serveur
	 * ***************************************************************************/

	log("CREATION D'UN CLIENT CONNECTE - client 0", __LINE__);
	interlocutorClient0.setName("proxyClient0");

	// V�rification de la branche root
	Branche& root = client0Tree.getRoot();
	ASSERT_EQUAL(rootName, root.getBrancheName(), "Le nom de la branche root du client est incorrect");
	ASSERT_EQUAL(0, root.getRevision(), "La r�vision initiale d'une branche devrait �tre nulle");

	// Connexion � l'arbre serveur
	distantClient0 = serverTree.addDistant(&interlocutorClient0);
	distantClient0->getControl().setUpdateClientToServerDelay(0);
	distantClient0->getControl().setUpdateServerToClientDelay(0);


	/* ****************************************************************************
	 * Client : Synchronisation et �change des donn�es du serveur vers le client
	 * => Le serveur envoie les changements vers le client
	 * ***************************************************************************/

	log("DIFFUSION DES DONNEES DU SERVEUR VERS CLIENT-0", __LINE__);

	serverTree.diffuseChangementsToClients();
	echangeDonneesClientServeur(__LINE__, interlocutorClient0);
	client0Tree.receiveChangementsFromServer();

	// V�rifie que le serveur et client-1 ont exactement le m�me arbre
	ASSERT_EQUAL(((AbstractBranche&)(serverTree.getRoot())).print(distantClient0, 0, false), ((AbstractBranche&)(client0Tree.getRoot())).print(0, 0, false), "Les abres client-0 et serveur devraient �tre identiques");

	{
		ostringstream arbre;
		arbre << endl << "ARBRE CLIENT 0 :";
		client0Tree.getRoot().print(arbre, 0, true, 0);
		log(arbre, __LINE__);
	}


	/* ****************************************************************************
	 * Client : V�rifie la branche 0
	 * ***************************************************************************/

	{
		Branche* branche0 = client0Tree.getBranche(client0Tree.getDistantServer(), branche0ServerFullId);

		cout << endl << "a:" << branche0->getMarqueur(client0Tree.getDistantServer()) << flush;

		ASSERT_EQUAL(branche0ServerName, branche0->getBrancheName(), "Le nom de la branche 0 est incorrect");
		ASSERT_EQUAL(0, branche0->getRevision(), "La r�vision initiale d'une branche devrait �tre nulle");
	}


	/* ****************************************************************************
	 * Client 0 : V�rifie les donn�es re�ues provenant de l'arbre du serveur
	 * ***************************************************************************/

	// Valeur string
	{
		ValeurString* valeurString = (ValeurString*)client0Tree.getValeur(client0Tree.getDistantServer(), branche0ServerFullId, valeurStringServerId);

		cout << endl << "A:" << valeurString->getMarqueur(client0Tree.getDistantServer()) << flush;

		ASSERT_EQUAL(valeurStringServerName, valeurString->getValeurName(), "Le nom de la valeur string est incorrect");
		ASSERT_EQUAL(0, valeurString->getRevision(), "La r�vision initiale d'une valeur devrait �tre nulle");
		ASSERT_EQUAL(valeurStringServerValue, valeurString->getValeur(), "La valeur de la valeur est fausse");
	}

	// Valeur int
	{
		ValeurInt* valeurInt = (ValeurInt*)client0Tree.getValeur(client0Tree.getDistantServer(), branche0ServerFullId, valeurIntServerId);

		cout << endl << "B:" << valeurInt->getMarqueur(client0Tree.getDistantServer()) << flush;

		ASSERT_EQUAL(valeurIntServerName, valeurInt->getValeurName(), "Le nom de la valeur string est incorrect");
		ASSERT_EQUAL(0, valeurInt->getRevision(), "La r�vision initiale d'une valeur devrait �tre nulle");
		ASSERT_EQUAL(valeurIntServerValue, valeurInt->getValeur(), "La valeur de la valeur est fausse");
	}

	// Valeur float
	{
		ValeurFloat* valeurFloat = (ValeurFloat*)client0Tree.getValeur(client0Tree.getDistantServer(), branche0ServerFullId, valeurFloatServerId);

		cout << endl << "C:" << valeurFloat->getMarqueur(client0Tree.getDistantServer());

		ASSERT_EQUAL(valeurFloatServerName, valeurFloat->getValeurName(), "Le nom de la valeur string est incorrect");
		ASSERT_EQUAL(0, valeurFloat->getRevision(), "La r�vision initiale d'une valeur devrait �tre nulle");
		ASSERT_EQUAL(valeurFloatServerValue, valeurFloat->getValeur(), "La valeur de la valeur est fausse");
	}


	echangeDonneesClientServeur(__LINE__, interlocutorClient0);

	serverTree.receiveChangementsFromClients();

	// V�rifie si client et serveur sont synchronis�s, plus aucun message ne devrait �tre � �changer entre eux
	checkSynchronisationClientServeur(__LINE__, client0Tree);


	/* ***************************************************************************
	 * Test de mise � jour de valeurs c�t� serveur
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

	// V�rifie si les valeurs ont bien �t� mises � jour c�t� client 0
	{
		ValeurString* valeurString = (ValeurString*)client0Tree.getValeur(0, branche0ServerFullId, valeurStringServerId);
		cout << endl << "-->" << valeurString->getValeurName();
		ASSERT_EQUAL(1, valeurString->getRevision(), "La r�vision aurait du �tre incr�ment�e");
		ASSERT_EQUAL(newValeurServeurStringValue, valeurString->getValeur(), "La valeur de la valeur est fausse ou  n'a pas �t� mise � jour sur le client 0");

		ValeurInt* valeurInt = (ValeurInt*)client0Tree.getValeur(0, branche0ServerFullId, valeurIntServerId);
		ASSERT_EQUAL(1, valeurInt->getRevision(), "La r�vision aurait du �tre incr�ment�e");
		ASSERT_EQUAL(newValeurServeurIntValue, valeurInt->getValeur(), "La valeur de la valeur est fausse ou  n'a pas �t� mise � jour sur le client 0");

		ValeurFloat* valeurFloat = (ValeurFloat*)client0Tree.getValeur(0, branche0ServerFullId, valeurFloatServerId);
		ASSERT_EQUAL(1, valeurFloat->getRevision(), "La r�vision aurait du �tre incr�ment�e");
		ASSERT_EQUAL(newValeurServeurFloatValue, valeurFloat->getValeur(), "La valeur de la valeur est fausse ou  n'a pas �t� mise � jour sur le client 0");
	}

	log("Envoi de la confirmation par le client", __LINE__);
	client0Tree.diffuseChangementsToServer();
	echangeDonneesClientServeur(__LINE__, interlocutorClient0);
	serverTree.receiveChangementsFromClients();

	// V�rifie si client et serveur sont synchronis�s, plus aucun message ne devrait �tre � �changer entre eux
	checkSynchronisationClientServeur(__LINE__, client0Tree);

	// V�rifie que le serveur et client-1 ont exactement le m�me arbre
	ASSERT_EQUAL(((AbstractBranche&)serverTree.getRoot()).print(distantClient0, 0, false), ((AbstractBranche&)client0Tree.getRoot()).print(0, 0, false), "Les abres client-0 et serveur devraient �tre identiques");


	/* ****************************************************************************
	 * Client : Ajout de donn�es c�t� client dans client 0
	 * ***************************************************************************/

	log("CREATION DE DONNEES SUR LE CLIENT", __LINE__);

	// Ajout branche 0
	Branche* branche0Client0 = client0Tree.createBranche(0, rootFullId, branche0Client0Name);

	// Ajout valeur 0
	ValeurInt* valeur0Client0 = (ValeurInt*)client0Tree.createValeur(0, ANY, branche0Client0->getBrancheFullIdOrTmpId(), valeur0Client0Name, AnyData(valeur0Client0Value));

	// Ajout branche 1
	Branche* branche1Client0 = client0Tree.createBranche(0, branche0Client0->getBrancheFullIdOrTmpId(), branche1Client0Name);

	// Ajout valeur 1
	ValeurInt* valeur1Client0 = (ValeurInt*)client0Tree.createValeur(0, ANY, branche1Client0->getBrancheFullIdOrTmpId(), valeur1Client0Name, AnyData(valeur1Client0Value));

	// Ajout branche 2
	Branche* branche2Client0 = client0Tree.createBranche(0, branche0Client0->getBrancheFullIdOrTmpId(), branche2Client0Name);

	// Ajout valeur 2
	ValeurInt* valeur2Client0 = (ValeurInt*)client0Tree.createValeur(0, ANY, branche2Client0->getBrancheFullIdOrTmpId(), valeur2Client0Name, AnyData(valeur2Client0Value));

	// Ajout valeur 3
	ValeurInt* valeur3Client0 = (ValeurInt*)client0Tree.createValeur(0, ANY, branche2Client0->getBrancheFullIdOrTmpId(), valeur3Client0Name, AnyData(valeur3Client0Value));

	{
		ostringstream arbre;
		arbre << endl << "ARBRE CLIENT 0 :";
		client0Tree.getRoot().print(arbre, 0, true, 0);
		log(arbre, __LINE__);
	}

	/* ****************************************************************************
	 * V�rifie si les identifiants sont bien initialis�s
	 * ***************************************************************************/

	ASSERT_EQUAL(-1, branche0Client0->getBrancheId(), "Identifiant mal initialis�");
	ASSERT_EQUAL(-1, branche0Client0->getBrancheTmpId(), "Identifiant temporaire mal initialis�");

	ASSERT_EQUAL(-1, valeur0Client0->getValeurId(), "Identifiant mal initialis�");
	ASSERT_EQUAL(-1, valeur0Client0->getValeurTmpId(), "Identifiant temporaire mal initialis�");

	ASSERT_EQUAL(-1, branche1Client0->getBrancheId(), "Identifiant mal initialis�");
	ASSERT_EQUAL(-1, branche1Client0->getBrancheTmpId(), "Identifiant temporaire mal initialis�");

	ASSERT_EQUAL(-1, valeur1Client0->getValeurId(), "Identifiant mal initialis�");
	ASSERT_EQUAL(-1, valeur1Client0->getValeurTmpId(), "Identifiant temporaire mal initialis�");

	ASSERT_EQUAL(-1, branche2Client0->getBrancheId(), "Identifiant mal initialis�");
	ASSERT_EQUAL(-2, branche2Client0->getBrancheTmpId(), "Identifiant temporaire mal initialis�");

	ASSERT_EQUAL(-1, valeur2Client0->getValeurId(), "Identifiant mal initialis�");
	ASSERT_EQUAL(-1, valeur2Client0->getValeurTmpId(), "Identifiant temporaire mal initialis�");

	ASSERT_EQUAL(-1, valeur3Client0->getValeurId(), "Identifiant mal initialis�");
	ASSERT_EQUAL(-2, valeur3Client0->getValeurTmpId(), "Identifiant temporaire mal initialis�");


	/* ****************************************************************************
	 * Serveur : Synchronisation et �change des donn�es du client vers le serveur
	 * => Le client envoie les changements vers le serveur
	 * ***************************************************************************/

	log("DIFFUSION DES DONNEES DU CLIENT", __LINE__);

	client0Tree.diffuseChangementsToServer();
	echangeDonneesClientServeur(__LINE__, interlocutorClient0);
	serverTree.receiveChangementsFromClients();

	{
		ostringstream arbre;
		arbre << "ARBRE SERVER :";
		serverTree.getRoot().print(arbre, 0, true, 0);
		log(arbre, __LINE__);
	}


	/* ****************************************************************************
	 * Serveur : V�rifie si le serveur a bien re�u les donn�es du client
	 * ***************************************************************************/

	// Branche 0
	Branche* branche0Client0S = serverTree.getBranche(0, branche0Client0FullId);
	ASSERT_NOT_NULL(branche0Client0S, "Le serveur n'a pas recu la branche");
	ASSERT_EQUAL(branche0Client0Name, branche0Client0S->getBrancheName(), "La branche recue par le serveur est mal nomm�e");

	// Valeur 0
	ValeurInt* valeur0Client0S = (ValeurInt*)serverTree.getValeur(0, branche0Client0FullId, valeur0Client0Id);
	ASSERT_NOT_NULL(valeur0Client0S, "Le serveur n'a pas recu la valeur");
	ASSERT_EQUAL(valeur0Client0Name, valeur0Client0S->getValeurName(), "La valeur recue par le serveur est mal nomm�e");
	ASSERT_EQUAL(valeur0Client0Value, valeur0Client0S->getValeur(), "La valeur recue par le serveur est mal nomm�e");

	// Branche 1
	Branche* branche1Client0S = serverTree.getBranche(0, branche1Client0FullId);
	ASSERT_NOT_NULL(branche1Client0S, "Le serveur n'a pas recu la branche");
	//	logDataTreeElementId("Identifiant", branche1Client0S->getBrancheFullId());
	ASSERT_EQUAL(branche1Client0Name, branche1Client0S->getBrancheName(), "La branche recue par le serveur est mal nomm�e");

	// Valeur 1
	ValeurInt* valeur1Client0S = (ValeurInt*)serverTree.getValeur(0, branche1Client0FullId, valeur1Client0Id);
	ASSERT_NOT_NULL(valeur1Client0S, "Le serveur n'a pas recu la valeur");
	ASSERT_EQUAL(valeur1Client0Name, valeur1Client0S->getValeurName(), "La valeur recue par le serveur est mal nomm�e");
	ASSERT_EQUAL(valeur1Client0Value, valeur1Client0S->getValeur(), "La valeur recue par le serveur est mal nomm�e");

	// Branche 2
	Branche* branche2Client0S = serverTree.getBranche(0, branche2Client0FullId);
	ASSERT_NOT_NULL(branche2Client0S, "Le serveur n'a pas recu la branche");
	ASSERT_EQUAL(branche2Client0Name, branche2Client0S->getBrancheName(), "La branche recue par le serveur est mal nomm�e");

	// Valeur 2
	ValeurInt* valeur2Client0S = (ValeurInt*)serverTree.getValeur(0, branche2Client0FullId, valeur2Client0Id);
	ASSERT_NOT_NULL(valeur2Client0S, "Le serveur n'a pas recu la valeur");
	ASSERT_EQUAL(valeur2Client0Name, valeur2Client0S->getValeurName(), "La valeur recue par le serveur est mal nomm�e");
	ASSERT_EQUAL(valeur2Client0Value, valeur2Client0S->getValeur(), "La valeur recue par le serveur est mal nomm�e");

	// Valeur 3
	ValeurInt* valeur3Client0S = (ValeurInt*)serverTree.getValeur(0, branche2Client0FullId, valeur3Client0Id);
	ASSERT_NOT_NULL(valeur3Client0S, "Le serveur n'a pas recu la valeur");
	ASSERT_EQUAL(valeur3Client0Name, valeur3Client0S->getValeurName(), "La valeur recue par le serveur est mal nomm�e");
	ASSERT_EQUAL(valeur3Client0Value, valeur3Client0S->getValeur(), "La valeur recue par le serveur est mal nomm�e");


	/* *******************************************************************
	 * Transmission au client 0 des acceptations de changements
	 * �mises par le serveur.
	 * Elles ont pour r�le de provoquer l'arr�t de l'envoi des changements
	 * par le client lorsqu'il comprend que le serveur les a bien pris
	 * en compte.
	 * ******************************************************************/

	log("DIFFUSION DES ACCEPTATIONS DU SERVEUR", __LINE__);

	ASSERT_EQUAL(0, interlocutorClient0.countDataReceived(), "Il ne devrait rester aucun message � recevoir");
	ASSERT_EQUAL(1, interlocutorClient0.countDataToSend(), "Un seul message contenant les acceptations aurait du avoir �t� envoy� par le serveur");
	ASSERT_EQUAL(0, interlocutorClient0.countTechnicalMessagesReceived(), "Il ne devrait rester aucun message technique � recevoir");
	ASSERT_EQUAL(0, interlocutorClient0.countTechnicalMessagesToSend(), "Il ne devrait rester aucun message technique � envoyer");

	echangeDonneesClientServeur(__LINE__, interlocutorClient0);

	ASSERT_EQUAL(1, interlocutorClient0.countDataReceived(), "Le message contenant les acceptations aurait du avoir �t� recu du client 0");
	ASSERT_EQUAL(0, interlocutorClient0.countDataToSend(), "Il ne devrait rester aucun message � envoyer");

	client0Tree.receiveChangementsFromServer();

	log("DIFFUSION DES CONFIRMATIONS DU CLIENT 0", __LINE__);

	ASSERT_EQUAL(0, interlocutorClient0.countDataReceived(), "Il ne devrait rester aucun message recu");
	ASSERT_EQUAL(1, interlocutorClient0.countDataToSend(), "Un seul message contenant les confirmations aurait du avoir �t� envoy� par le client 0");

	echangeDonneesClientServeur(__LINE__, interlocutorClient0);

	ASSERT_EQUAL(1, interlocutorClient0.countDataReceived(), "Le message contenant les confirmations aurait du avoir �t� recu du serveur");
	ASSERT_EQUAL(0, interlocutorClient0.countDataToSend(), "Il ne devrait rester aucun message � envoyer");

	serverTree.receiveChangementsFromClients();

	// V�rifie si client et serveur sont synchronis�s, plus aucun message ne devrait �tre � �changer entre eux
	checkSynchronisationClientServeur(__LINE__, client0Tree);

	// V�rifie que le serveur et client-0 ont exactement le m�me arbre
	ASSERT_EQUAL(((AbstractBranche&)serverTree.getRoot()).print(0, 0, false), ((AbstractBranche&)client0Tree.getRoot()).print(0, 0, false), "Les abres client-0 et serveur devraient �tre identiques");
}

void DataTreeTest::multiClientsTests() {
	/* ****************************************************************************
	 * Client : Cr�ation d'un second client (client-1) connect� au serveur
	 * ***************************************************************************/

	log("CREATION D'UN SECOND CLIENT CONNECTE - client 1", __LINE__);
	interlocutorClient1.setName("proxyClient1");

	// Connexion � l'arbre serveur
	distantClient1 = serverTree.addDistant(&interlocutorClient1);

	log("DIFFUSION DES DONNEES DU SERVEUR VERS CLIENT-1", __LINE__);

	// Envoi des donn�es de l'arbre serveur au client
	serverTree.diffuseChangementsToClients();
	echangeDonneesClientServeur(__LINE__, interlocutorClient1);
	client1Tree.receiveChangementsFromServer();

	// Traitement des confirmation que la client a bien re�u les donn�es
	client1Tree.diffuseChangementsToServer();
	echangeDonneesClientServeur(__LINE__, interlocutorClient1);
	serverTree.receiveChangementsFromClients();

	// L'arbre doit �tre stabilit� ici
	checkSynchronisationClientServeur(__LINE__, client1Tree);

	// V�rifie que le serveur et client-1 ont exactement le m�me arbre
	ASSERT_EQUAL(((AbstractBranche&)serverTree.getRoot()).print(0, 0, false), ((AbstractBranche&)client1Tree.getRoot()).print(0, 0, false), "Les abres client-1 et serveur devraient �tre identiques");

	// Les arbres doivent d�sormais �tre stabilis�s
	checkSynchronisationClientServeur(__LINE__, client0Tree);
	checkSynchronisationClientServeur(__LINE__, client1Tree);


	/* ****************************************************************************
	 * Cr�ation de donn�es par client-1
	 * ***************************************************************************/

	log("CREATION DE DONNEES PAR CLIENT 1", __LINE__);

	client1Tree.createValeur(0, ANY, branche0ServerFullId, "valeur-0-client1", AnyData(41));
	client1Tree.createValeur(0, ANY, branche0ServerFullId, "valeur-1-client1", AnyData("Yoyo"));
	client1Tree.createValeur(0, ANY, branche2Client0FullId, "valeur-2-client1", AnyData(1.0235f));
	Branche* branche0Client1 = client1Tree.createBranche(0, branche2Client0FullId, "branche-0-client1");
	client1Tree.createValeur(0, ANY, branche0Client1->getBrancheFullIdOrTmpId(), "valeur-3-client1", AnyData("Hummm"));


	// La nouvelle valeur de client 1 descend vers le serveur
	client1Tree.diffuseChangementsToServer();
	echangeDonneesClientServeur(__LINE__, interlocutorClient1);
	serverTree.receiveChangementsFromClients();

	// Le serveur diffuse la valeur de client 1 vers client 0 et informe client 1 de l'identifiant d�finitif de la valeur
	serverTree.diffuseChangementsToClients();
	echangeDonneesClientServeur(__LINE__, interlocutorClient0);
	echangeDonneesClientServeur(__LINE__, interlocutorClient1);
	client0Tree.receiveChangementsFromServer();
	client1Tree.receiveChangementsFromServer();

	// Client 0 et client 1 confirment au serveur qu'ils ont bien re�u les informations
	client0Tree.diffuseChangementsToServer();
	client1Tree.diffuseChangementsToServer();
	echangeDonneesClientServeur(__LINE__, interlocutorClient0);
	echangeDonneesClientServeur(__LINE__, interlocutorClient1);
	serverTree.receiveChangementsFromClients();

	// Les arbres doivent d�sormais �tre stabilis�s
	checkSynchronisationClientServeur(__LINE__, client0Tree);
	checkSynchronisationClientServeur(__LINE__, client1Tree);

	// Test l'�galit� des 3 arbres
	ASSERT_EQUAL(((AbstractBranche&)client1Tree.getRoot()).print(0, 0, false), ((AbstractBranche&)client0Tree.getRoot()).print(0, 0, false), "Les abres client-1 et serveur devraient �tre identiques");
	ASSERT_EQUAL(((AbstractBranche&)client1Tree.getRoot()).print(0, 0, false), ((AbstractBranche&)serverTree.getRoot()).print(0, 0, false), "Les abres client-1 et client-0 devraient �tre identiques");

	{
		ostringstream arbre;
		arbre << "ARBRE SERVER :";
		serverTree.getRoot().print(arbre, 0, true, 0);
		log(arbre, __LINE__);
	}



	/* ****************************************************************************
	 * Cr�ation de donn�es par client 0 et client 1 aux m�mes endroits
	 * ***************************************************************************/

	log("CREATION DE DONNEES PAR 2 CLIENTS SIMULTANEMENT", __LINE__);

	client0Tree.createValeur(0, ANY, branche0ServerFullId, "valeur-10-client0", AnyData(44));
	client1Tree.createValeur(0, ANY, branche0ServerFullId, "valeur-11-client1", AnyData(45));

	Branche* branche12Client0 = client0Tree.createBranche(0, branche2Client0FullId, "branche-12-client0");
	client0Tree.createValeur(0, ANY, branche12Client0->getBrancheFullIdOrTmpId(), "valeur-12-client0", AnyData("Minou"));
	Branche* branche13Client1 = client1Tree.createBranche(0, branche2Client0FullId, "branche-13-client1");
	client1Tree.createValeur(0, ANY, branche13Client1->getBrancheFullIdOrTmpId(), "valeur-13-client1", AnyData("Poilu"));

	// Les nouvelles donn�es de client 1 et  client 2 descendent vers le serveur
	log("Envoi des donn�es par les clients", __LINE__);
	client0Tree.diffuseChangementsToServer();
	client1Tree.diffuseChangementsToServer();

	log("R�ception des donn�es par le serveur et envoi des acceptations", __LINE__);
	echangeDonneesClientServeur(__LINE__, interlocutorClient0);
	echangeDonneesClientServeur(__LINE__, interlocutorClient1);
	serverTree.receiveChangementsFromClients();

	// Diffusion des nouvelles donn�es par le serveur
	log("Diffusion des nouvelles donn�es par le serveur", __LINE__);
	serverTree.diffuseChangementsToClients();
	echangeDonneesClientServeur(__LINE__, interlocutorClient0);
	echangeDonneesClientServeur(__LINE__, interlocutorClient1);

	// R�ception des nouvelles donn�es et des accceptions par les clients et envoi des confirmations
	log("R�ception des nouvelles donn�es et des accceptions par les clients et envoi des confirmations", __LINE__);
	client0Tree.receiveChangementsFromServer();
	client1Tree.receiveChangementsFromServer();

	// R�ception des confirmations par le serveur
	log("R�ception des confirmations par le serveur", __LINE__);
	echangeDonneesClientServeur(__LINE__, interlocutorClient0);
	echangeDonneesClientServeur(__LINE__, interlocutorClient1);
	serverTree.receiveChangementsFromClients();

	// Les arbres doivent d�sormais �tre stabilis�s
	checkSynchronisationClientServeur(__LINE__, client0Tree);
	checkSynchronisationClientServeur(__LINE__, client1Tree);

	// Test l'�galit� des 3 arbres
	ASSERT_EQUAL(((AbstractBranche&)client1Tree.getRoot()).print(0, 0, false), ((AbstractBranche&)client0Tree.getRoot()).print(0, 0, false), "Les abres client-1 et client-0 devraient �tre identiques");
	ASSERT_EQUAL(((AbstractBranche&)client1Tree.getRoot()).print(0, 0, false), ((AbstractBranche&)serverTree.getRoot()).print(0, 0, false), "Les abres client-1 et serveur devraient �tre identiques");

	{
		ostringstream arbre;
		arbre << "ARBRE SERVER :";
		serverTree.getRoot().print(arbre, 0, true, 0);
		log(arbre, __LINE__);
	}
}

void DataTreeTest::privateTreeTest() {
	log("CREATION D'UNE BRANCHE PRIVEE", __LINE__);

	PrivateBranche* privateBranche = (PrivateBranche*)serverTree.createPrivateBranche(rootFullId, branche1ServerName);


	/* **********************************************************************************************************
	 * Cr�ation de donn�es priv�es sur Client 0
	 * *********************************************************************************************************/

	log("CREATION DE DONNEES SUR LA BRANCHE PRIVEE DE CLIENT 0", __LINE__);

	Valeur* privClient0Value = serverTree.createValeur(distantClient0, ANY, privateBranche->getBrancheFullId(), "Client 0 - pr val", AnyData("Toutcru"));
	Branche* privClient0Br = serverTree.createBranche(distantClient0, privateBranche->getBrancheFullId(), "Client 0 - pr br");

	{
		ostringstream arbre;
		arbre << "ARBRE SERVER :";
		serverTree.getRoot().print(arbre, 0, true, 0);
		log(arbre, __LINE__);
	}


	log("DIFFUSION DES DONNEES DU SERVEUR VERS CLIENT-0 et CLIENT-1", __LINE__);

	serverTree.diffuseChangementsToClients();

	echangeDonneesClientServeur(__LINE__, interlocutorClient0);
	client0Tree.receiveChangementsFromServer();
	client0Tree.diffuseChangementsToServer();
	echangeDonneesClientServeur(__LINE__, interlocutorClient0);

	echangeDonneesClientServeur(__LINE__, interlocutorClient1);
	client1Tree.receiveChangementsFromServer();
	client1Tree.diffuseChangementsToServer();
	echangeDonneesClientServeur(__LINE__, interlocutorClient1);

	serverTree.receiveChangementsFromClients();

	// Les arbres doivent d�sormais �tre stabilis�s
	checkSynchronisationClientServeur(__LINE__, client0Tree);
	checkSynchronisationClientServeur(__LINE__, client1Tree);

	{
		ostringstream arbre;
		arbre << "ARBRE CLIENT 0 :";
		client0Tree.getRoot().print(arbre, 0, true, 0);
		log(arbre, __LINE__);
	}

	{
		ostringstream arbre;
		arbre << "ARBRE CLIENT 1 :";
		client1Tree.getRoot().print(arbre, 0, true, 0);
		log(arbre, __LINE__);
	}

	// V�rifications sur Client 0
	ASSERT_EQUAL(DONNEE_PRIVATE, client0Tree.getBranche(0, privateBranche->getBrancheFullId())->getDonneeType(), "La branche devrait �tre priv�e c�t� client comme c�t� serveur");
	ASSERT_EQUAL(DONNEE_PRIVATE_SUB, client0Tree.getBranche(0, privClient0Br->getBrancheFullId())->getDonneeType(), "La branche devrait �tre sub-priv�e c�t� client comme c�t� serveur");
	ASSERT_EQUAL(DONNEE_PRIVATE_SUB, client0Tree.getValeur(0, privClient0Value->getBrancheId(), privClient0Value->getValeurId())->getDonneeType(), "La branche devrait �tre sub-priv�e c�t� client comme c�t� serveur");

	// V�rifications sur Client 1
	ASSERT_EQUAL(DONNEE_PRIVATE, client1Tree.getBranche(0, privateBranche->getBrancheFullId())->getDonneeType(), "La branche devrait �tre priv�e c�t� client comme c�t� serveur");
	try {
		ASSERT_EQUAL(DONNEE_PRIVATE_SUB, client1Tree.getBranche(0, privClient0Br->getBrancheFullId())->getDonneeType(), "La branche devrait �tre sub-priv�e c�t� client comme c�t� serveur");
		ASSERT_TRUE(false, "This branche shouldn't exist on client 1 but only on client 0");
	}
	catch(NotExistingBrancheException& exception) {
		// All right
	}
	try {
		ASSERT_EQUAL(DONNEE_PRIVATE_SUB, client1Tree.getValeur(0, privClient0Value->getBrancheId(), privClient0Value->getValeurId())->getDonneeType(), "La branche devrait �tre sub-priv�e c�t� client comme c�t� serveur");
		ASSERT_TRUE(false, "This value shouldn't exist on client 1 but only on client 0");
	}
	catch(NotExistingValeurException& exception) {
		// All right
	}


	/* **********************************************************************************************************
	 * Cr�ation de donn�es priv�es sur Client 1
	 * *********************************************************************************************************/

	log("CREATION DE DONNEES SUR LA BRANCHE PRIVEE DE CLIENT 1", __LINE__);
	Valeur* privClient1Value1 = serverTree.createValeur(distantClient1, ANY, privateBranche->getBrancheFullId(), "Private value 1 client 1", AnyData("Vache"));
	Branche* privClient1Br1 = serverTree.createBranche(distantClient1, privateBranche->getBrancheFullId(), "Private branche 1 client 1");
	Valeur* privClient1Value2 = serverTree.createValeur(distantClient1, ANY, privClient1Br1->getBrancheFullId(), "Private value 2 client 1", AnyData("Panda"));
	Branche* privClient1Br2 = serverTree.createBranche(distantClient1, privClient1Br1->getBrancheFullId(), "Private branche 2 client 1");

	{
		ostringstream arbre;
		arbre << "ARBRE SERVER :";
		serverTree.getRoot().print(arbre, 0, true, 0);
		log(arbre, __LINE__);
	}


	log("DIFFUSION DES DONNEES DU SERVEUR VERS CLIENT-0 et CLIENT-1", __LINE__);

	serverTree.diffuseChangementsToClients();

	echangeDonneesClientServeur(__LINE__, interlocutorClient0);
	client0Tree.receiveChangementsFromServer();
	client0Tree.diffuseChangementsToServer();
	echangeDonneesClientServeur(__LINE__, interlocutorClient0);

	echangeDonneesClientServeur(__LINE__, interlocutorClient1);
	client1Tree.receiveChangementsFromServer();
	client1Tree.diffuseChangementsToServer();
	echangeDonneesClientServeur(__LINE__, interlocutorClient1);

	serverTree.receiveChangementsFromClients();

	// Les arbres doivent d�sormais �tre stabilis�s
	checkSynchronisationClientServeur(__LINE__, client0Tree);
	checkSynchronisationClientServeur(__LINE__, client1Tree);

	{
		ostringstream arbre;
		arbre << "ARBRE CLIENT 0 :";
		client0Tree.getRoot().print(arbre, 0, true, 0);
		log(arbre, __LINE__);
	}

	{
		ostringstream arbre;
		arbre << "ARBRE CLIENT 1 :";
		client1Tree.getRoot().print(arbre, 0, true, 0);
		log(arbre, __LINE__);
	}

	// V�rifications sur Client 0
	ASSERT_EQUAL(DONNEE_PRIVATE, client0Tree.getBranche(0, privateBranche->getBrancheFullId())->getDonneeType(), "La branche devrait �tre priv�e c�t� client comme c�t� serveur");
	ASSERT_EQUAL(DONNEE_PRIVATE_SUB, client0Tree.getBranche(0, privClient0Br->getBrancheFullId())->getDonneeType(), "La branche devrait �tre sub-priv�e c�t� client comme c�t� serveur");
	ASSERT_EQUAL(DONNEE_PRIVATE_SUB, client0Tree.getValeur(0, privClient0Value->getBrancheId(), privClient0Value->getValeurId())->getDonneeType(), "La valeur devrait �tre sub-priv�e c�t� client comme c�t� serveur");

	// V�rifications sur Client 1
	ASSERT_EQUAL(DONNEE_PRIVATE, client1Tree.getBranche(0, privateBranche->getBrancheFullId())->getDonneeType(), "La branche devrait �tre priv�e c�t� client comme c�t� serveur");
	ASSERT_EQUAL(DONNEE_PRIVATE_SUB, client1Tree.getBranche(0, privClient1Br1->getBrancheFullId())->getDonneeType(), "La branche devrait �tre sub-priv�e c�t� client comme c�t� serveur");
	ASSERT_EQUAL(DONNEE_PRIVATE_SUB, client1Tree.getValeur(0, privClient1Value1->getBrancheId(), privClient1Value1->getValeurId())->getDonneeType(), "La valeur devrait �tre sub-priv�e c�t� client comme c�t� serveur");
	ASSERT_EQUAL(DONNEE_PRIVATE_SUB, client1Tree.getValeur(0, privClient1Value2->getBrancheId(), privClient1Value2->getValeurId())->getDonneeType(), "La valeur devrait �tre sub-priv�e c�t� client comme c�t� serveur");
	ASSERT_EQUAL(DONNEE_PRIVATE_SUB, client1Tree.getBranche(0, privClient1Br2->getBrancheFullId())->getDonneeType(), "La branche devrait �tre sub-priv�e c�t� client comme c�t� serveur");
}

void DataTreeTest::echangeDonneesClientServeur(int line, Interlocutor2& client) {
	JktUtils::Bytes* data;
	int count = 0;


	while((data = client.popDataToSend())) {
		client.pushDataReceived(data);
		++count;
	};

	ostringstream msg;
	msg << "Nombre de paquets �chang�s entre " << client.getName() << " et le serveur : " << count;
	log(msg, line);
}

void DataTreeTest::checkSynchronisationClientServeur(int line, ClientDataTree& clientTree) {
	log("VERIFICATION DE LA SYNCHRONISATION CLIENT SERVEUR avec " + clientTree.getClientName(), line);

	serverTree.diffuseChangementsToClients();
	assertEqual(__FILE__, line, 0, clientTree.getDistantServer()->getInterlocutor()->countDataReceived(), "countDataReceived 1: Il ne devrait rester aucun message � recevoir");
	assertEqual(__FILE__, line, 0, clientTree.getDistantServer()->getInterlocutor()->countDataToSend(), "countDataToSend 1: Il ne devrait rester aucun message � envoyer");
	assertEqual(__FILE__, line, 0, clientTree.getDistantServer()->getInterlocutor()->countTechnicalMessagesReceived(), "countTechnicalMessagesReceived 1: Il ne devrait rester aucun message technique � recevoir");
	assertEqual(__FILE__, line, 0, clientTree.getDistantServer()->getInterlocutor()->countTechnicalMessagesToSend(), "countTechnicalMessagesToSend 1: Il ne devrait rester aucun message technique � envoyer");

	serverTree.receiveChangementsFromClients();
	assertEqual(__FILE__, line, 0, clientTree.getDistantServer()->getInterlocutor()->countDataReceived(), "countDataReceived 2: Il ne devrait rester aucun message � recevoir");
	assertEqual(__FILE__, line, 0, clientTree.getDistantServer()->getInterlocutor()->countDataToSend(), "countDataToSend 2: Il ne devrait rester aucun message � envoyer");
	assertEqual(__FILE__, line, 0, clientTree.getDistantServer()->getInterlocutor()->countTechnicalMessagesReceived(), "countTechnicalMessagesReceived 2: Il ne devrait rester aucun message technique � recevoir");
	assertEqual(__FILE__, line, 0, clientTree.getDistantServer()->getInterlocutor()->countTechnicalMessagesToSend(), "countTechnicalMessagesToSend 2: Il ne devrait rester aucun message technique � envoyer");

	clientTree.diffuseChangementsToServer();
	assertEqual(__FILE__, line, 0, clientTree.getDistantServer()->getInterlocutor()->countDataReceived(), "countDataReceived 3: Il ne devrait rester aucun message � recevoir");
	assertEqual(__FILE__, line, 0, clientTree.getDistantServer()->getInterlocutor()->countDataToSend(), "countDataToSend 3: Il ne devrait rester aucun message � envoyer");
	assertEqual(__FILE__, line, 0, clientTree.getDistantServer()->getInterlocutor()->countTechnicalMessagesReceived(), "countTechnicalMessagesReceived 3: Il ne devrait rester aucun message technique � recevoir");
	assertEqual(__FILE__, line, 0, clientTree.getDistantServer()->getInterlocutor()->countTechnicalMessagesToSend(), "countTechnicalMessagesToSend 3: Il ne devrait rester aucun message technique � envoyer");

	clientTree.receiveChangementsFromServer();
	assertEqual(__FILE__, line, 0, clientTree.getDistantServer()->getInterlocutor()->countDataReceived(), "countDataReceived 4: Il ne devrait rester aucun message � recevoir");
	assertEqual(__FILE__, line, 0, clientTree.getDistantServer()->getInterlocutor()->countDataToSend(), "countDataToSend 4: Il ne devrait rester aucun message � envoyer");
	assertEqual(__FILE__, line, 0, clientTree.getDistantServer()->getInterlocutor()->countTechnicalMessagesReceived(), "countTechnicalMessagesReceived 4: Il ne devrait rester aucun message technique � recevoir");
	assertEqual(__FILE__, line, 0, clientTree.getDistantServer()->getInterlocutor()->countTechnicalMessagesToSend(), "countTechnicalMessagesToSend 4: Il ne devrait rester aucun message technique � envoyer");
}

} /* namespace JktTest */
