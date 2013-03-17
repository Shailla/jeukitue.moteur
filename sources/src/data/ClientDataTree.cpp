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
#include "data/exception/DataCommunicationException.h"
#include "data/communication/DataSerializer.h"
#include "data/communication/message/AddBrancheChangement.h"
#include "util/CollectionsUtils.h"

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
		try {
			if(AddBrancheChangement* addChgt = dynamic_cast<AddBrancheChangement*>(*itCh)) {
				Branche* parentBranche = getBranche(addChgt->getParentBrancheId());

				if(parentBranche) {
					parentBranche->addSubBranche(addChgt->getBrancheId(), addChgt->getBrancheName(), addChgt->getRevision());
				}
				else {
					cerr << endl << "Branche parent inexistante";
				}
			}
		}
		catch(const NotExistingBrancheException& exception) {
			cerr << endl << "Exception : NotExistingBrancheException";
		}
		catch(const DataCommunicationException& exception) {
			cerr << endl << "Exception : " << exception.getMessage();
		}
	}
}
