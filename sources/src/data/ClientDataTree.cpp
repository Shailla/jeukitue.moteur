/*
 * ClientDataTree.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

#include "util/TraceMethod.h"
#include "util/Trace.h"
#include "util/StringUtils.h"
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
#include "data/DistantTreeProxy.h"

#include "data/ClientDataTree.h"

using namespace JktUtils;

ClientDataTree::ClientDataTree(const std::string& clientName, Interlocutor2* serverInterlocutor) {
	_clientName = clientName;
	_serverTreeProxy = new DistantTreeProxy(serverInterlocutor);
}

ClientDataTree::~ClientDataTree() {
}

const string& ClientDataTree::getClientName() const {
	return _clientName;
}

void ClientDataTree::initDistantBranche(DistantTreeProxy* distant, Branche* branche) {
}

Branche* ClientDataTree::createBranche(const std::vector<int>& parentBrancheId, const std::string& brancheName) {
	Branche* parentBranche = getBranche(parentBrancheId);
	Branche* branche = parentBranche->createSubBrancheForClient(brancheName, 0);
	initDonneeAndServeurMarqueur(branche);

	return branche;
}

Valeur* ClientDataTree::createValeur(const std::vector<int>& parentBrancheId, const std::string& valeurName, const AnyData value) {
	Branche* parentBranche = getBranche(parentBrancheId);
	Valeur* valeur = parentBranche->createValeurForClient(valeurName, 0, value);
	initDonneeAndServeurMarqueur(valeur);

	return valeur;
}

void ClientDataTree::initDonneeAndServeurMarqueur(Donnee* donnee) {
	_donnees.insert(donnee);

	_serverTreeProxy->addMarqueur(donnee, 0);
}

DistantTreeProxy* ClientDataTree::getDistantServer() const {
	return _serverTreeProxy;
}

void ClientDataTree::diffuseChangementsToServer(void) {
	vector<Changement*> changements;

	_serverTreeProxy->collecteChangementsInClientTree(changements);

	sendChangementsToServer(changements);
}

Branche* ClientDataTree::getBrancheByTmpId(const vector<int>& parentBrancheId, int brancheTmpId) throw(NotExistingBrancheException) {
	vector<int>::const_iterator iter;

	Branche* parentBranche = &getRoot();

	for(iter = parentBrancheId.begin() ; (iter != parentBrancheId.end() && parentBranche != NULL) ; iter++) {
		parentBranche = parentBranche->getSubBrancheByIdOrTmpId(*iter);
	}

	if(!parentBranche) {
		throw NotExistingBrancheException("ClientDataTree::getBrancheByTmpId 1");
	}

	Branche* branche = parentBranche->getSubBrancheByIdOrTmpId(brancheTmpId);

	if(!branche) {
		throw NotExistingBrancheException("ClientDataTree::getBrancheByTmpId 2");
	}

	return branche;
}

void ClientDataTree::receiveChangementsFromServer() {
	TRACEMETHOD();

	try {
		vector<Changement*> answers;

		/* ********************************************
		 * Traitement des données venant du serveur
		 * *******************************************/

		Interlocutor2* interlocutor = _serverTreeProxy->getInterlocutor();

		// Récupération des données du serveur
		JktUtils::Bytes* fromServer;

		while((fromServer = interlocutor->popDataReceived())) {
			vector<Changement*> changements;

			istringstream in(string(fromServer->getBytes(), fromServer->size()));
			DataSerializer::fromStream(changements, in);
			delete fromServer;

			vector<Changement*>::iterator itCh;

			std::sort(changements.begin(), changements.end(), Changement::highestPriority);

			for(itCh = changements.begin() ; itCh != changements.end() ; itCh++) {
				TRACE().info("'%s' from '%s' : '%s'", _clientName.c_str(), interlocutor->getName().c_str(), (*itCh)->toString().c_str());

				try {
					// Le serveur accepte la création de la nouvelle branche demandée par ce client et lui attribue son identifiant définitif
					if(AcceptAddBrancheFromClientChangement* chgt = dynamic_cast<AcceptAddBrancheFromClientChangement*>(*itCh)) {
						Branche* parentBranche = getBranche(chgt->getParentBrancheId());
						Branche* branche = parentBranche->acceptTmpSubBranche(chgt->getBrancheTmpId(), chgt->getBrancheId(), chgt->getRevision());

						answers.push_back(new ConfirmBrancheChangement(branche->getBrancheFullId(), branche->getRevision()));
					}

					// Le serveur accepte la création de la nouvelle branche demandée par ce client et lui attribue son identifiant définitif
					else if(AcceptAddValeurFromClientChangement* chgt = dynamic_cast<AcceptAddValeurFromClientChangement*>(*itCh)) {
						Branche* branche = getBranche(chgt->getParentBrancheId());
						Valeur* valeur = branche->acceptTmpValeur(chgt->getValeurTmpId(), chgt->getValeurId(), chgt->getRevision());

						MarqueurDistant* marqueur = _serverTreeProxy->getMarqueur(valeur);
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
						Branche* parent = getBranche(chgt->getParentBrancheId());
						const Valeur* valeur = parent->addValeur(chgt->getValeurId(), chgt->getValeurName(), chgt->getRevision(), chgt->getValeur());

						answers.push_back(new ConfirmValeurChangement(valeur->getBrancheId(), valeur->getValeurId(), valeur->getRevision()));
					}

					// Le serveur informe de la modification d'une valeur
					else if(UpdateValeurFromServerChangement* chgt = dynamic_cast<UpdateValeurFromServerChangement*>(*itCh)) {
						Valeur* valeur = getValeur(chgt->getParentBrancheId(), chgt->getValeurId());
						valeur->setValeur(chgt->getRevision(), chgt->getValeur());

						answers.push_back(new ConfirmValeurChangement(valeur->getBrancheId(), valeur->getValeurId(), valeur->getRevision()));
					}

					// Le server confirme la révision de valeur qu'il a reçu
					else if(ConfirmValeurChangement* chgt = dynamic_cast<ConfirmValeurChangement*>(*itCh)) {
						Valeur* valeur = getValeur(chgt->getBrancheId(), chgt->getValeurId());
						MarqueurDistant* marqueur = _serverTreeProxy->getMarqueur(valeur);
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

				delete *itCh;
			}
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
	TRACEMETHOD();

	vector<Changement*>::iterator iter;
	Interlocutor2* interlocutor = _serverTreeProxy->getInterlocutor();

	if(changements.size()) {
		for(iter = changements.begin() ; iter != changements.end() ; iter++) {
			TRACE().info("'%s' to '%s' : '%s'", _clientName.c_str(), interlocutor->getName().c_str(), (*iter)->toString().c_str());
		}

		ostringstream out;
		DataSerializer::toStream(changements, out);

		for(iter = changements.begin() ; iter != changements.end() ; iter++) {
			delete *iter;
		}

		Bytes* toSend = new Bytes(out.str());
		interlocutor->pushDataToSend(toSend);
	}
}
