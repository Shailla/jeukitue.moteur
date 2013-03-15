/*
 * DataTree.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

#include "data/MarqueurClient.h"
#include "data/exception/NotExistingBrancheException.h"
#include "data/communication/DataSerializer.h"
#include "data/communication/message/AddBrancheChangement.h"

#include "data/ClientDataTree.h"

ClientDataTree::ClientDataTree() : _root(NULL, 0, "root") {
}

ClientDataTree::~ClientDataTree() {
}

void ClientDataTree::receiveChangements(const string& data) {
	vector<Changement*> changements;

	istringstream in(data);
	DataSerializer::fromStream(changements, in);

	vector<Changement*>::iterator itCh;

	for(itCh = changements.begin() ; itCh != changements.end() ; itCh++) {
		if(AddBrancheChangement* addChgt = dynamic_cast<AddBrancheChangement*>(*itCh)) {
			vector<int>& parentBrancheId = addChgt->getParentBrancheId();

			Branche* parentBranche = getBranche(parentBrancheId);

			if() {
				parentBranche->addSubBranche(addChgt->getBrancheId(), addChgt->getBrancheName(), addChgt->getRevision());
			}
			else {
				cerr << endl << "Branche parent inexistante";
			}
		}
	}
}
