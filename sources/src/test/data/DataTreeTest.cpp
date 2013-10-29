/*
 * DataTreeTest.cpp
 *
 *  Created on: 29 oct. 2013
 *      Author: Erwin
 */

#include "reseau/new/Interlocutor2.h"
#include "data/ServeurDataTree.h"
#include "data/ClientDataTree.h"

#include "test/data/DataTreeTest.h"

namespace JktTest {

DataTreeTest::DataTreeTest() : Test("DataTreeTest") {
}

DataTreeTest::~DataTreeTest() {
}

void DataTreeTest::test() {

	/* ********************************************************
	 * Test data
	 * ********************************************************/

	const string rootName = "root";
	vector<int> rootFullId;

	int branche0Id = 0;
	vector<int> branche0FullId;
	branche0FullId.push_back(branche0Id);
	const string branche0Name = "branche-0";


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

	serverTree.createBranche(rootFullId, branche0Name);
	Branche* branche0 = serverTree.getBranche(branche0FullId);
	ASSERT_EQUAL(branche0Name, branche0->getBrancheName(), "Le nom de la branche 0 est incorrect");

}

} /* namespace JktTest */
