/*
 * DataTreeTest.h
 *
 *  Created on: 29 oct. 2013
 *      Author: Erwin
 */

#ifndef DATATREETEST_H_
#define DATATREETEST_H_

#include <string>

#include "reseau/new/Interlocutor2.h"
#include "data/ServeurDataTree.h"
#include "data/ClientDataTree.h"
#include "data/ValeurString.h"
#include "data/ValeurFloat.h"
#include "data/ValeurInt.h"

#include "test/Test.h"

namespace jkt {

class DataTreeTest : public Test {
	Interlocutor2 interlocutorClient0;
	Interlocutor2 interlocutorClient1;

	ServeurDataTree serverTree;

	ClientDataTree client0Tree;
	ClientDataTree client1Tree;

	DistantTreeProxy* distantClient0;
	DistantTreeProxy* distantClient1;

	// Données de test
	/* ------------------ Global data ------------------ */

	// Server branche root
	const std::string rootName = "root";
	std::vector<int> rootFullId;

	/* ------------------ Server data ------------------ */

	// Server branche 0
	const int branche0ServerId = 2;
	std::vector<int> branche0ServerFullId;
	const std::string branche0ServerName = "branche-0-server";

	// Server valeur string
	const int valeurStringServerId = 1;
	std::vector<int> valeurStringServerFullId;
	const std::string valeurStringServerName = "valeur-string-server";
	const std::string valeurStringServerValue = "valeur-string-server-value";

	// Server valeur int
	const int valeurIntServerId = 2;
	std::vector<int> valeurIntServerFullId;
	const std::string valeurIntServerName = "valeur-int-server";
	const int valeurIntServerValue = 637;

	// Server valeur float
	const int valeurFloatServerId = 3;
	std::vector<int> valeurFloatServerFullId;
	const std::string valeurFloatServerName = "valeur-float-server";
	const float valeurFloatServerValue = 6.83f;

	// Server branche 1
	const int branche1ServerId = 3;
	std::vector<int> branche1ServerFullId;
	const std::string branche1ServerName = "PRIVATE-BRANCHE-1-server";

	/* ------------------ Client 0 data ------------------ */

	// Branche 0
	const int branche0Client0Id = 3;
	std::vector<int> branche0Client0FullId;
	const std::string branche0Client0Name = "branche-0-client-0";

	// Valeur 0 (dans la branche 0)
	const int valeur0Client0Id = 1;
	std::vector<int> valeur0Client0FullId;
	const std::string valeur0Client0Name = "valeur-0-client-0";
	const int valeur0Client0Value = 927;

	// Branche 1 (dans la branche 0)
	const int branche1Client0Id = 1;
	std::vector<int> branche1Client0FullId;
	const std::string branche1Client0Name = "branche-1-client-0";

	// Valeur 1 (dans la branche 1)
	const int valeur1Client0Id = 1;
	std::vector<int> valeur1Client0FullId;
	const std::string valeur1Client0Name = "valeur-1-client-0";
	const int valeur1Client0Value = 524;

	// Branche 2 (dans la branche 0)
	const int branche2Client0Id = 2;
	std::vector<int> branche2Client0FullId;
	const std::string branche2Client0Name = "branche-2-client-0";

	// Valeur 2 (dans la branche 2)
	const int valeur2Client0Id = 1;
	std::vector<int> valeur2Client0FullId;
	const std::string valeur2Client0Name = "valeur-2-client-0";
	const int valeur2Client0Value = 332;

	// Valeur 3 (dans la branche 2)
	int valeur3Client0Id = 2;
	std::vector<int> valeur3Client0FullId;
	const std::string valeur3Client0Name = "valeur-3-client-0";
	const int valeur3Client0Value = 547;

	// Valeur 4 (dans la branche 2)
	int valeur4Client0Id = 2;
	std::vector<int> valeur4Client0FullId;
	const std::string valeur4Client0Name = "valeur-4-client-0";
	const int valeur4Client0Value = 742;

	Branche* serveurRoot = 0;
	ValeurString* valeurServeurString = 0;
	ValeurInt* valeurServeurInt = 0;
	ValeurFloat* valeurServeurFloat = 0;

	/** Compare 2 branches, renvoie true si elles sont égales, false sinon. */
	bool isEqualBranche(Branche& branche1, Branche& branche2);

	void checkSynchronisationClientServeur(int line, ClientDataTree& client0Tree);

	/** Réalise les échanges de données entre un client et le serveur. */
	void echangeDonneesClientServeur(int line, Interlocutor2& client);
public:
	DataTreeTest();
	virtual ~DataTreeTest();

	std::string getDescription() override;

	void test() override;

private:
	void logDataTreeElementId(const std::string dataTreeElementName, const std::vector<int>& dataTreeElementId);
	void initTestData();
	void serverTests();
	void clientTests();
	void iteratorTest();
	void multiClientsTests();
	void privateTreeTest();
};

} /* namespace jkt */

#endif /* DATATREETEST_H_ */
