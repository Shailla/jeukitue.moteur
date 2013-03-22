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
#include "data/communication/message/AddBrancheFromServerChangement.h"
#include "data/communication/message/AddValeurFromServerChangement.h"
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

Branche* ClientDataTree::createBranche(std::vector<int>& parentBrancheId, const std::string& brancheName) {
	Branche* parentBranche = getBranche(parentBrancheId);
	Branche* branche = parentBranche->createSubBrancheForClient(brancheName);
	addServeurMarqueur(branche);

	return branche;
}

Valeur* ClientDataTree::createValeurInt(std::vector<int>& parentBrancheId, const std::string& valeurName, int value) {
	Branche* parentBranche = getBranche(parentBrancheId);
	Valeur* valeur = parentBranche->createValeurIntForClient(valeurName, value);
	addServeurMarqueur(valeur);

	return valeur;
}

void ClientDataTree::addServeurMarqueur(Donnee* donnee) {
	_serveur->addMarqueur(donnee, 0);
}

Distant* ClientDataTree::getDistantServer() const {
	return _serveur;
}

void ClientDataTree::diffuseChangementsToServer(void) {
	vector<Distant*>::iterator clientIter;
	vector<Changement*> changements;

	_serveur->collecteChangementsInClientTree(changements);

	if(changements.size()) {
		ostringstream out;
		DataSerializer::toStream(changements, out);
		_serveur->setDataToServer(new string(out.str()));
	}
}

void ClientDataTree::receiveChangementsFromServer(const string& data, vector<Changement*>& confirmations) {
	vector<Changement*> changements;

	istringstream in(data);
	DataSerializer::fromStream(changements, in);

	vector<Changement*>::iterator itCh;

	for(itCh = changements.begin() ; itCh != changements.end() ; itCh++) {
		try {
			if(AddBrancheFromServerChangement* chgt = dynamic_cast<AddBrancheFromServerChangement*>(*itCh)) {
				Branche* parentBranche = getBranche(chgt->getParentBrancheId());

				if(parentBranche) {
					Branche* branche = parentBranche->addSubBranche(chgt->getBrancheId(), chgt->getBrancheName(), chgt->getRevision());
					confirmations.push_back(new ConfirmBrancheChangement(branche->getBrancheFullId(), branche->getRevision()));
				}
				else {
					cerr << endl << __FILE__ << ":" << __LINE__ << " Branche parent inexistante";
				}
			}
			else if(AddValeurFromServerChangement* chgt = dynamic_cast<AddValeurFromServerChangement*>(*itCh)) {
				Branche* parent = getBranche(chgt->getBrancheId());

				if(parent) {
					Valeur* valeur = parent->addValeurInt(chgt->getValeurId(), chgt->getValeurName(), chgt->getRevision(), chgt->getValeur());
					confirmations.push_back(new ConfirmValeurChangement(valeur->getBrancheId(), valeur->getValeurId(), valeur->getRevision()));
				}
				else {
					cerr << endl << __FILE__ << ":" << __LINE__ << " Branche parent inexistante";
				}
			}
			else if(UpdateValeurChangement* chgt = dynamic_cast<UpdateValeurChangement*>(*itCh)) {
				Valeur* valeur = getValeur(chgt->getBrancheId(), chgt->getValeurId());

				if(valeur) {
					valeur->setValeur(chgt->getRevision(), chgt->getValeur());
					confirmations.push_back(new ConfirmValeurChangement(valeur->getBrancheId(), valeur->getValeurId(), valeur->getRevision()));
				}
				else {
					cerr << endl << __FILE__ << ":" << __LINE__ << " Valeur inexistante";
				}
			}
		}
		catch(const NotExistingBrancheException& exception) {
			cerr << endl << __FILE__ << ":" << __LINE__ << " Exception : NotExistingBrancheException";
		}
		catch(const DataCommunicationException& exception) {
			cerr << endl << __FILE__ << ":" << __LINE__ << " Exception : " << exception.getMessage();
		}
	}
}

string* ClientDataTree::sendChangementsToServer(vector<Changement*>& changements) {
	vector<Changement*>::iterator iter;

	for(iter = changements.begin() ; iter != changements.end() ; iter++) {
		cout << (*iter)->toString();
	}

	if(changements.size()) {
		ostringstream out;
		DataSerializer::toStream(changements, out);

		return new string(out.str());
	}
	else {
		return NULL;
	}
}
