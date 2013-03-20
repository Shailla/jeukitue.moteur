/*
 * ClientDataTree.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

#include "data/MarqueurDistant.h"
#include "data/exception/NotExistingBrancheException.h"
#include "data/exception/DataCommunicationException.h"
#include "data/communication/DataSerializer.h"
#include "data/communication/message/AddBrancheChangement.h"
#include "data/communication/message/AddValeurChangement.h"
#include "data/communication/message/UpdateValeurChangement.h"
#include "data/communication/message/ConfirmBrancheChangement.h"
#include "data/communication/message/ConfirmValeurChangement.h"
#include "util/CollectionsUtils.h"

#include "data/ClientDataTree.h"

ClientDataTree::ClientDataTree(Distant* server) {
	_serveur = server;
}

ClientDataTree::~ClientDataTree() {
}

void ClientDataTree::initDistantBranche(Distant* distant, Branche* branche) {

}

Distant* ClientDataTree::getDistantServer() const {
	return _serveur;
}

void ClientDataTree::receiveChangementsFromServer(const string& data, vector<Changement*>& confirmations) {
	vector<Changement*> changements;

	istringstream in(data);
	DataSerializer::fromStream(changements, in);

	vector<Changement*>::iterator itCh;

	for(itCh = changements.begin() ; itCh != changements.end() ; itCh++) {
		try {
			if(AddBrancheChangement* chgt = dynamic_cast<AddBrancheChangement*>(*itCh)) {
				Branche* parentBranche = getBranche(chgt->getParentBrancheId());

				if(parentBranche) {
					Branche* branche = parentBranche->addSubBranche(chgt->getBrancheId(), chgt->getBrancheName(), chgt->getRevision());
					confirmations.push_back(new ConfirmBrancheChangement(branche->getBrancheFullId(), branche->getRevision()));
				}
				else {
					cerr << endl << "Branche parent inexistante";
				}
			}
			else if(AddValeurChangement* chgt = dynamic_cast<AddValeurChangement*>(*itCh)) {
				Branche* parent = getBranche(chgt->getBrancheId());

				if(parent) {
					Valeur* valeur = parent->addValeurInt(chgt->getValeurId(), chgt->getValeurName(), chgt->getRevision(), chgt->getValeur());
					confirmations.push_back(new ConfirmValeurChangement(valeur));
				}
				else {
					cerr << endl << "Branche parent inexistante";
				}
			}
			else if(UpdateValeurChangement* chgt = dynamic_cast<UpdateValeurChangement*>(*itCh)) {
				Valeur* valeur = getValeur(chgt->getBrancheId(), chgt->getValeurId());

				if(valeur) {
					valeur->setValeur(chgt->getRevision(), chgt->getValeur());
					confirmations.push_back(new ConfirmValeurChangement(valeur));
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

void ClientDataTree::sendChangementsToServer(vector<Changement*>& changements) {
	if(changements.size()) {
		ostringstream out;
		DataSerializer::toStream(changements, out);
		_serveur->setDataToServer(out);
	}
}
