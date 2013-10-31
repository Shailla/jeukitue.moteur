/*
 * DataTreeTest.cpp
 *
 *  Created on: 29 oct. 2013
 *      Author: Erwin
 */

#include "util/types/StringData.h"
#include "reseau/new/Interlocutor2.h"
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

	// Data valeur string
	int valeurStringId = 0;
	vector<int> valeurStringFullId = branche0FullId;
	valeurStringFullId.push_back(valeurStringId);
	const string valeurStringName = "valeur-string";


	/* ****************************************************************************
	 * Création du serveur
	 * ***************************************************************************/
	ServeurDataTree serverTree;

	Branche& root = serverTree.getRoot();
	ASSERT_EQUAL(rootName, root.getBrancheName(), "Le nom de la branche 0 est incorrect");
	ASSERT_EQUAL(0, root.getRevision(), "La révision initiale d'une branche devrait être nulle");


	/* ****************************************************************************
	 * Création du client
	 * ***************************************************************************/
	Interlocutor2 clientInterlocutor(SDL_CreateCond(), SDL_CreateMutex());
	ClientDataTree clientTree("client-test", &clientInterlocutor);


	/* ****************************************************************************
	 * Test d'ajout d'une branche
	 * ***************************************************************************/

	Branche* branche0Tmp = serverTree.createBranche(rootFullId, branche0Name);
	Branche* branche0 = serverTree.getBranche(branche0FullId);

	ASSERT_EQUAL(branche0Tmp, branche0, "La branche créée et celle lue devraient être les mêmes");
	ASSERT_EQUAL(branche0Name, branche0->getBrancheName(), "Le nom de la branche 0 est incorrect");
	ASSERT_EQUAL(0, branche0->getRevision(), "La révision initiale d'une branche devrait être nulle");


	/* ****************************************************************************
	 * Test d'ajout d'une valeur
	 * ***************************************************************************/

	Valeur* valeurStringTmp = serverTree.createValeur(branche0FullId, valeurStringName, new StringData("Coucou"));
	Valeur* valeurString = serverTree.getValeur(branche0FullId, valeurStringId);

	ASSERT_EQUAL(valeurStringTmp, valeurString, "La valeur créée et celle lue devraient être les mêmes");
	ASSERT_EQUAL(valeurStringName, valeurString->getValeurName(), "Le nom de la valeur string est incorrect");
	ASSERT_EQUAL(0, valeurString->getRevision(), "La révision initiale d'une valeur devrait être nulle");
}

} /* namespace JktTest */
