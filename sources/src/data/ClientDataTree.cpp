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

#include "util/types/IntData.h"
#include "data/ValeurInt.h"
#include "data/MarqueurDistant.h"
#include "data/exception/NotExistingBrancheException.h"
#include "data/exception/DataCommunicationException.h"
#include "data/communication/DataSerializer.h"
#include "data/communication/message/ServerToClient/AddBrancheFromServerChangement.h"
#include "data/communication/message/ServerToClient/AddValeurFromServerChangement.h"
#include "data/communication/message/ServerToClient/AcceptAddBrancheFromClientChangement.h"
#include "data/communication/message/ServerToClient/AcceptAddValeurFromClientChangement.h"
#include "data/communication/message/ServerToClient/UpdateValeurFromServerChangement.h"
#include "data/communication/message/ConfirmBrancheChangement.h"
#include "data/communication/message/ConfirmValeurChangement.h"
#include "util/CollectionsUtils.h"

#include "data/ClientDataTree.h"

ClientDataTree::ClientDataTree(Distant* server, const string& clientName) {
	_clientName = clientName;;
	_serveur = server;
}

ClientDataTree::~ClientDataTree() {
}

const string& ClientDataTree::getClientName() const {
	return _clientName;
}

void ClientDataTree::initDistantBranche(Distant* distant, Branche* branche) {

}

Branche* ClientDataTree::createBranche(const std::vector<int>& parentBrancheId, const std::string& brancheName, int revision) {
	Branche* parentBranche = getBranche(parentBrancheId);
	Branche* branche = parentBranche->createSubBrancheForClient(brancheName, revision);
	addServeurMarqueur(branche);

	return branche;
}

Valeur* ClientDataTree::createValeurInt(const std::vector<int>& parentBrancheId, const std::string& valeurName, int revision, int value) {
	Branche* parentBranche = getBranche(parentBrancheId);
	Valeur* valeur = parentBranche->createValeurIntForClient(valeurName, revision, value);
	addServeurMarqueur(valeur);

	return valeur;
}

Valeur* ClientDataTree::updateValeurInt(const std::vector<int>& brancheId, int valeurId, int value) {
	ValeurInt* valeur = (ValeurInt*)getValeur(brancheId, valeurId);
	valeur->updateValeur(value);

	return valeur;
}

void ClientDataTree::addServeurMarqueur(Donnee* donnee) {
	_serveur->addMarqueur(donnee, 0);
}

Distant* ClientDataTree::getDistantServer() const {
	return _serveur;
}

void ClientDataTree::diffuseChangementsToServer(void) {
	vector<Changement*> changements;

	_serveur->collecteChangementsInClientTree(changements);

	sendChangementsToServer(changements);
}

Branche* ClientDataTree::getBrancheByTmpId(const vector<int>& parentBrancheId, int brancheTmpId) throw(NotExistingBrancheException) {
	vector<int>::const_iterator iter;

	Branche* parentBranche = &getRoot();

	for(iter = parentBrancheId.begin() ; (iter != parentBrancheId.end() && parentBranche != NULL) ; iter++) {
		parentBranche = parentBranche->getSubBranche(*iter);
	}

	if(!parentBranche) {
		throw NotExistingBrancheException();
	}

	Branche* branche = parentBranche->getSubBrancheByTmpId(brancheTmpId);

	if(!branche) {
		throw NotExistingBrancheException();
	}

	return branche;
}

void ClientDataTree::receiveChangementsFromServer() {
	try {
		vector<Changement*> changements;
		vector<Changement*> answers;

		/* ********************************************
		 * Traitement des données venant du serveur
		 * *******************************************/

		// Récupération des données du serveur
		string* fromServer = _serveur->getDataReceived();

		if(fromServer) {
			istringstream in(*fromServer);
			DataSerializer::fromStream(changements, in);

			vector<Changement*>::iterator itCh;

			for(itCh = changements.begin() ; itCh != changements.end() ; itCh++) {
				cout << endl << _clientName << " from " << _serveur->getDebugName() << "\t : " << (*itCh)->toString() << flush;

				try {
					// Le serveur accepte la création de la nouvelle branche demandée par ce client et lui attribue son identifiant définitif
					if(AcceptAddBrancheFromClientChangement* chgt = dynamic_cast<AcceptAddBrancheFromClientChangement*>(*itCh)) {
						Branche* parentBranche = getBranche(chgt->getParentBrancheId());
						Branche* branche = parentBranche->acceptTmpSubBranche(chgt->getBrancheTmpId(), chgt->getBrancheId(), chgt->getRevision());

						answers.push_back(new ConfirmBrancheChangement(branche->getBrancheFullId(), branche->getRevision()));
					}

					// Le serveur accepte la création de la nouvelle branche demandée par ce client et lui attribue son identifiant définitif
					if(AcceptAddValeurFromClientChangement* chgt = dynamic_cast<AcceptAddValeurFromClientChangement*>(*itCh)) {
						Branche* branche = getBranche(chgt->getBrancheId());
						Valeur* valeur = branche->acceptTmpValeur(chgt->getValeurTmpId(), chgt->getValeurId(), chgt->getRevision());

						MarqueurDistant* marqueur = _serveur->getMarqueur(valeur);
						marqueur->setConfirmedRevision(chgt->getRevision());

						answers.push_back(new ConfirmValeurChangement(valeur->getBrancheId(), valeur->getValeurId(), branche->getRevision()));
					}

					// Le serveur informe de la création d'une nouvelle branche
					else if(AddBrancheFromServerChangement* chgt = dynamic_cast<AddBrancheFromServerChangement*>(*itCh)) {
						Branche* parentBranche = getBranche(chgt->getParentBrancheId());
						Branche* branche = parentBranche->addSubBranche(chgt->getBrancheId(), chgt->getBrancheName(), chgt->getRevision());

						answers.push_back(new ConfirmBrancheChangement(branche->getBrancheFullId(), branche->getRevision()));
					}

					// Le serveur informe de la création d'une nouvelle valeur
					else if(AddValeurFromServerChangement* chgt = dynamic_cast<AddValeurFromServerChangement*>(*itCh)) {
						Branche* parent = getBranche(chgt->getBrancheId());
						Valeur* valeur = parent->addValeurInt(chgt->getValeurId(), chgt->getValeurName(), chgt->getRevision(), chgt->getValeur());

						answers.push_back(new ConfirmValeurChangement(valeur->getBrancheId(), valeur->getValeurId(), valeur->getRevision()));
					}

					// Le serveur informe de la modification d'une valeur
					else if(UpdateValeurFromServerChangement* chgt = dynamic_cast<UpdateValeurFromServerChangement*>(*itCh)) {
						Valeur* valeur = getValeur(chgt->getBrancheId(), chgt->getValeurId());
						valeur->setValeur(chgt->getRevision(), chgt->getValeur());

						answers.push_back(new ConfirmValeurChangement(valeur->getBrancheId(), valeur->getValeurId(), valeur->getRevision()));
					}

					// Le server confirme la révision de valeur qu'il a reçu
					else if(ConfirmValeurChangement* chgt = dynamic_cast<ConfirmValeurChangement*>(*itCh)) {
						Valeur* valeur = getValeur(chgt->getBrancheId(), chgt->getValeurId());
						MarqueurDistant* marqueur = _serveur->getMarqueur(valeur);
						marqueur->setConfirmedRevision(chgt->getRevision());
					}

					else {
						throw DataCommunicationException();
					}
				}
				catch(const NotExistingBrancheException& exception) {
					cerr << endl << __FILE__ << ":" << __LINE__ << " Exception : NotExistingBrancheException";
				}
				catch(const DataCommunicationException& exception) {
					cerr << endl << __FILE__ << ":" << __LINE__ << " Exception : " << exception.getMessage();
				}
			}

			delete fromServer;
		}


		/* ********************************************
		 * Envoi des confirmations au serveur
		 * *******************************************/

		// Envoi les changements présents sur le client au serveur
		sendChangementsToServer(answers);
	}
	catch(DataCommunicationException& exception) {
		cerr << endl << __FILE__ << ":" << __LINE__ << " Exception : " << exception.getMessage();
	}
}

void ClientDataTree::sendChangementsToServer(vector<Changement*>& changements) {
	vector<Changement*>::iterator iter;

	if(changements.size()) {
		for(iter = changements.begin() ; iter != changements.end() ; iter++) {
			cout << endl << _clientName << " to " << _serveur->getDebugName() << "\t : " << (*iter)->toString() << flush;
		}

		ostringstream out;
		DataSerializer::toStream(changements, out);
		_serveur->setDataToSend(new string(out.str()));
	}
}
