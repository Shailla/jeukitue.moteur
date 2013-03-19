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
#include "data/communication/message/AddValeurChangement.h"
#include "data/communication/message/UpdateValeurChangement.h"
#include "util/CollectionsUtils.h"

#include "data/ClientDataTree.h"

ClientDataTree::ClientDataTree() {
}

ClientDataTree::~ClientDataTree() {
}

void ClientDataTree::initDistantBranche(Client* client, Branche* branche) {

}

void ClientDataTree::receiveChangements(const string& data) {
	vector<Changement*> changements;

	istringstream in(data);
	DataSerializer::fromStream(changements, in);

	vector<Changement*>::iterator itCh;

	for(itCh = changements.begin() ; itCh != changements.end() ; itCh++) {
		try {
			if(AddBrancheChangement* chgt = dynamic_cast<AddBrancheChangement*>(*itCh)) {
				Branche* parentBranche = getBranche(chgt->getParentBrancheId());

				if(parentBranche) {
					parentBranche->addSubBranche(chgt->getBrancheId(), chgt->getBrancheName(), chgt->getRevision());
				}
				else {
					cerr << endl << "Branche parent inexistante";
				}
			}
			else if(AddValeurChangement* chgt = dynamic_cast<AddValeurChangement*>(*itCh)) {
				Branche* parent = getBranche(chgt->getBrancheId());

				if(parent) {
					parent->addValeurInt(chgt->getValeurId(), chgt->getValeurName(), chgt->getRevision(), chgt->getValeur());
				}
				else {
					cerr << endl << "Branche parent inexistante";
				}
			}
			else if(UpdateValeurChangement* chgt = dynamic_cast<UpdateValeurChangement*>(*itCh)) {
				Valeur* valeur = getValeur(chgt->getBrancheId(), chgt->getValeurId());

				if(valeur) {
					valeur->setValeur(chgt->getRevision(), chgt->getValeur());
				}
				else {
					cerr << endl << "Valeur inexistante";
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
