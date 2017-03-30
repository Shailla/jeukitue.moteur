/*
 * ClientDataTree.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#include <iostream>
#include <vector>
#include <sstream>

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
#include "data/communication/message/ClientToServer/AddBrancheFromClientChangement.h"
#include "data/communication/message/ClientToServer/AddValeurFromClientChangement.h"
#include "data/communication/message/ClientToServer/UpdateValeurFromClientChangement.h"
#include "data/communication/message/ConfirmBrancheChangement.h"
#include "data/communication/message/ConfirmValeurChangement.h"
#include "util/CollectionsUtils.h"
#include "data/DataTreeUtils.h"
#include "data/DistantTreeProxy.h"

#include "data/ClientDataTree.h"

using namespace std;
using namespace jkt;

ClientDataTree::ClientDataTree(const std::string& clientName, Interlocutor2* serverInterlocutor) : DataTree(TREE_CLIENT) {
	_clientName = clientName;
	_serverTreeProxy = new DistantTreeProxy(serverInterlocutor);
	_updateClientToServer = SDL_GetTicks();

	/********************************************************************/
	/* Branche des VID (Very Important Data) de l'arbre de données 		*/
	/********************************************************************/

	// Branche racine
	vector<int> rootBrancheId;

	// Branche principale des VID
	Branche* rootBranche = getBranche(_serverTreeProxy, rootBrancheId);
	Branche* vidBranche = rootBranche->addSubBranche(_serverTreeProxy, 1, DONNEE_TYPE::DONNEE_DEFAULT, DataTreeUtils::TREE_VID_BRANCHE_NAME, 0);

	// Branche de contrôle de l'arbre de données
	vidBranche->addSubBranche(_serverTreeProxy, 1, DONNEE_TYPE::DONNEE_PRIVATE, DataTreeUtils::TREE_CONTROL_BRANCHE_NAME, 0);
//	createPrivateBranche(vidBranche->getBrancheFullId(), DataTreeUtils::TREE_CONTROL_BRANCHE_NAME);

	DistantTreeControl::initVid(this, _serverTreeProxy);
	initDistantBranche(_serverTreeProxy, &getRoot());
}

ClientDataTree::~ClientDataTree() {
}

const string& ClientDataTree::getClientName() const {
	return _clientName;
}

void ClientDataTree::initDistantBranche(DistantTreeProxy* ignored, Branche* branche) {
	MarqueurDistant* marqueur = _serverTreeProxy->addMarqueur(branche, 0);

	if(branche == &getRoot()) {	// Do not add the root branche to the distants, because it's a default existing element
		marqueur->setSentRevision(0);
		marqueur->setConfirmedRevision(0);
	}

	if(dynamic_cast<PrivateBranche*>(branche)) {
		PrivateBranche* pr = static_cast<PrivateBranche*>(branche);
		map<DistantTreeProxy*, PrivateBranche::DistantPrivateBranche>::iterator dt;

		for(dt = pr->getDistants().begin() ; dt != pr->getDistants().end() ; dt++) {
			// Init sub-branches
			{
				vector<Branche*>* subBranches = pr->getSubBranches(dt->first);
				vector<Branche*>::iterator itBr;

				for(itBr = subBranches->begin() ; itBr != subBranches->end() ; itBr++) {
					initDistantBranche(_serverTreeProxy, *itBr);
				}
			}

			// Init values
			{
				vector<Valeur*>& valeurs = pr->getValeurs(dt->first);
				vector<Valeur*>::iterator iterVl;

				for(iterVl = valeurs.begin() ; iterVl != valeurs.end() ; iterVl++) {
					_serverTreeProxy->addMarqueur(*iterVl, 0);
				}
			}
		}
	}
	else {
		// Init sub-branches
		{
			vector<Branche*>* subBranches = branche->getSubBranches(_serverTreeProxy);
			vector<Branche*>::iterator itBr;

			for(itBr = subBranches->begin() ; itBr != subBranches->end() ; itBr++) {
				initDistantBranche(_serverTreeProxy, *itBr);
			}
		}

		// Init values
		{
			vector<Valeur*>& valeurs = branche->getValeurs(_serverTreeProxy);
			vector<Valeur*>::iterator iterVl;

			for(iterVl = valeurs.begin() ; iterVl != valeurs.end() ; iterVl++) {
				_serverTreeProxy->addMarqueur(*iterVl, 0);
			}
		}
	}
}

Branche* ClientDataTree::createBranche(DistantTreeProxy* ignored, const std::vector<int>& parentBrancheId, const std::string& brancheName) {
	Branche* parentBranche = getBranche(_serverTreeProxy, parentBrancheId);
	Branche* branche = parentBranche->createSubBrancheForClient(brancheName, 0);
	initDonneeAndServeurMarqueur(branche);

	return branche;
}

Branche* ClientDataTree::getBrancheFromDistant(DistantTreeProxy* ignored, const vector<int>& brancheId) throw(NotExistingBrancheException) {
	vector<int>::const_iterator iter;

	Branche* branche = &getRoot();

	for(iter = brancheId.begin() ; (iter != brancheId.end() && branche != NULL) ; iter++) {
		int id = *iter;
		branche = branche->getSubBrancheByIdOrDistantTmpId(_serverTreeProxy, id);
	}

	if(branche == NULL) {
		throw NotExistingBrancheException("ClientDataTree::getBrancheFromDistant");
	}

	return branche;
}

Valeur* ClientDataTree::getValeurFromDistant(DistantTreeProxy* ignored, const vector<int>& brancheId, int valeurId) throw(NotExistingValeurException, NotExistingBrancheException) {
	vector<int>::const_iterator iter;

	Branche* branche = &getRoot();

	if(brancheId.size() > 0) {
		for(iter = brancheId.begin() ; (iter != brancheId.end() && branche != NULL) ; iter++) {
			branche = branche->getSubBrancheByIdOrDistantTmpId(_serverTreeProxy, *iter);
		}

		if(branche == NULL) {
			throw NotExistingBrancheException("DataTree::getValeurFromDistant 1");
		}
	}

	Valeur* valeur = branche->getValeurByIdOrTmpId(_serverTreeProxy, valeurId);

	if(!valeur) {
		throw NotExistingValeurException("DataTree::getValeurFromDistant 2");
	}

	return valeur;
}

Branche* ClientDataTree::createPrivateBranche(const std::vector<int>& parentBrancheId, const std::string& brancheName) {
	Branche* parentBranche = getBranche(_serverTreeProxy, parentBrancheId);
	Branche* branche = parentBranche->createSubBrancheForClient(brancheName, 0);
	initDonneeAndServeurMarqueur(branche);

	return branche;
}

Valeur* ClientDataTree::createValeur(DistantTreeProxy* ignored, UPDATE_MODE updateMode, const std::vector<int>& parentBrancheId, const std::string& valeurName, const AnyData value) {
	Branche* parentBranche = getBranche(_serverTreeProxy, parentBrancheId);
	Valeur* valeur = parentBranche->createValeurForClient(updateMode, valeurName, 0, value);
	initDonneeAndServeurMarqueur(valeur);

	return valeur;
}

MarqueurDistant* ClientDataTree::initDonneeAndServeurMarqueur(Donnee* donnee) {
	_donnees.insert(donnee);

	return _serverTreeProxy->addMarqueur(donnee, 0);
}

DistantTreeProxy* ClientDataTree::getDistantServer() const {
	return _serverTreeProxy;
}

void ClientDataTree::diffuseChangementsToServer(void) {
	vector<Changement*> changements;

	Uint32 now = SDL_GetTicks();

	if(now - _updateClientToServer >= (Uint32)_serverTreeProxy->getControl().getUpdateClientToServerDelay()) {
		_updateClientToServer = now;
		collecteChangements(changements);
		sendChangementsToServer(changements);
	}
}

void ClientDataTree::collecteChangements(vector<Changement*>& changements) {
	BrancheIterator it = getRoot().begin(_serverTreeProxy);

	Branche* branche;
	MarqueurDistant* marqueur;
	Changement* changement;

	while(++it) {
		branche = *it;

		// NOUVELLE BRANCHE : branche présente sur le client dont le serveur n'a pas encore connaissance (donc avec un identifiant temporaire)
		if(branche->getBrancheId() < 0) {
			changement = new AddBrancheFromClientChangement(branche->getParentBrancheIdOrTmpId(), branche->getBrancheTmpId(), branche->getRevision(), branche->getBrancheName());

			marqueur = branche->getMarqueur(_serverTreeProxy);
			changement->update(marqueur);
			changements.push_back(changement);
		}

		vector<Valeur*>& valeurs = branche->getValeurs(_serverTreeProxy);

		for(Valeur* valeur : valeurs) {
			changement = 0;
			marqueur = valeur->getMarqueur(_serverTreeProxy);

			// NOUVELLE VALEUR : valeur présente sur le client dont le serveur n'a pas encore connaissance (donc avec un identifiant temporaire)
			if(valeur->getValeurId() < 0) {
				changement = new AddValeurFromClientChangement(valeur->getBrancheIdOrTmpId(), valeur->getValeurTmpId(), valeur->getRevision(), valeur->getValeurName(), valeur->getValeurData());
			}

			// UPDATE VALEUR : Une valeur a changé et le client n'en a pas encore connaissance
			else if(valeur->getRevision() > marqueur->getConfirmedRevision()) {
				changement = new UpdateValeurFromClientChangement(valeur->getBrancheIdOrTmpId(), valeur->getValeurId(), valeur->getRevision(), valeur->getValeurData());
			}

			if(changement) {
				changement->update(marqueur);
				changements.push_back(changement);
			}
		}
	}
}

Branche* ClientDataTree::getBrancheByTmpId(const vector<int>& parentBrancheId, int brancheTmpId) throw(NotExistingBrancheException) {
	vector<int>::const_iterator iter;

	Branche* parentBranche = &getRoot();

	for(iter = parentBrancheId.begin() ; (iter != parentBrancheId.end() && parentBranche != NULL) ; iter++) {
		parentBranche = (Branche*)parentBranche->getSubBrancheByIdOrTmpId(_serverTreeProxy, *iter);	// Un arbre client a des vraies  branches, pas besoin de passer par AbstractBranche
	}

	if(!parentBranche) {
		throw NotExistingBrancheException("ClientDataTree::getBrancheByTmpId 1");
	}

	Branche* branche = (Branche*)parentBranche->getSubBrancheByIdOrTmpId(_serverTreeProxy, brancheTmpId);

	if(!branche) {
		throw NotExistingBrancheException("ClientDataTree::getBrancheByTmpId 2");
	}

	return branche;
}

void ClientDataTree::receiveChangementsFromServer() {
	try {
		vector<Changement*> answers;
		Uint32 time;
		saveTime(time);

		/* ********************************************
		 * Traitement des données venant du serveur
		 * *******************************************/

		Interlocutor2* interlocutor = _serverTreeProxy->getInterlocutor();

		// Récupération des données du serveur
		jkt::Bytes* fromServer;

		while((fromServer = interlocutor->popDataReceived())) {
			vector<Changement*> changements;
			vector<Changement*>::iterator itCh;

			istringstream in(string(fromServer->getBytes(), fromServer->size()));
			DataSerializer::fromStream(changements, in);
			delete fromServer;

			std::sort(changements.begin(), changements.end(), Changement::highestPriority);

			for(itCh = changements.begin() ; itCh != changements.end() ; itCh++) {
				LOGINFO(("'%s' from '%s' : '%s'", _clientName.c_str(), interlocutor->getName().c_str(), (*itCh)->toString().c_str()));

				try {
					// Le serveur accepte la création de la nouvelle branche demandée par ce client et lui attribue son identifiant définitif
					if(AcceptAddBrancheFromClientChangement* chgt = dynamic_cast<AcceptAddBrancheFromClientChangement*>(*itCh)) {
						Branche* parentBranche = getBranche(_serverTreeProxy, chgt->getParentBrancheId());
						Branche* branche = (Branche*)parentBranche->acceptTmpSubBranche(_serverTreeProxy, chgt->getBrancheTmpId(), chgt->getBrancheId(), chgt->getRevision());

						answers.push_back(new ConfirmBrancheChangement(branche->getBrancheFullId(), branche->getRevision()));
					}

					// Le serveur accepte la création de la nouvelle branche demandée par ce client et lui attribue son identifiant définitif
					else if(AcceptAddValeurFromClientChangement* chgt = dynamic_cast<AcceptAddValeurFromClientChangement*>(*itCh)) {
						Branche* branche = getBranche(_serverTreeProxy, chgt->getParentBrancheId());
						Valeur* valeur = branche->acceptTmpValeur(_serverTreeProxy, chgt->getValeurTmpId(), chgt->getValeurId(), chgt->getRevision());

						MarqueurDistant* marqueur = valeur->getMarqueur(_serverTreeProxy);
						marqueur->setConfirmedRevision(chgt->getRevision());

						answers.push_back(new ConfirmValeurChangement(valeur->getBrancheId(), valeur->getValeurId(), branche->getRevision()));
					}

					// Le serveur informe de la création d'une nouvelle branche
					else if(AddBrancheFromServerChangement* chgt = dynamic_cast<AddBrancheFromServerChangement*>(*itCh)) {
						Branche* parentBranche = getBranche(_serverTreeProxy, chgt->getParentBrancheId());
						Branche* branche = (Branche*)parentBranche->addSubBranche(_serverTreeProxy, chgt->getBrancheId(), chgt->getBrancheType(), chgt->getBrancheName(), chgt->getRevision());

						MarqueurDistant* marqueur = initDonneeAndServeurMarqueur(branche);
						chgt->update(marqueur);

						answers.push_back(new ConfirmBrancheChangement(branche->getBrancheFullId(), branche->getRevision()));
					}

					// Le serveur informe de la création d'une nouvelle valeur
					else if(AddValeurFromServerChangement* chgt = dynamic_cast<AddValeurFromServerChangement*>(*itCh)) {
						Branche* parent = getBranche(_serverTreeProxy, chgt->getParentBrancheId());
						Valeur* valeur = parent->addValeur(_serverTreeProxy, chgt->getUpdateMode(), chgt->getValeurId(), chgt->getValeurName(), chgt->getRevision(), chgt->getValeur());

						MarqueurDistant* marqueur = initDonneeAndServeurMarqueur(valeur);
						chgt->update(marqueur);

						answers.push_back(new ConfirmValeurChangement(valeur->getBrancheId(), valeur->getValeurId(), valeur->getRevision()));
					}

					// Le serveur informe de la modification d'une valeur
					else if(UpdateValeurFromServerChangement* chgt = dynamic_cast<UpdateValeurFromServerChangement*>(*itCh)) {
						Valeur* valeur = getValeur(_serverTreeProxy, chgt->getParentBrancheId(), chgt->getValeurId());
						valeur->setValeur(chgt->getRevision(), chgt->getValeur());

						MarqueurDistant* marqueur = valeur->getMarqueur(_serverTreeProxy);
						chgt->update(marqueur);
						marqueur->setConfirmedRevision(chgt->getRevision());

						answers.push_back(new ConfirmValeurChangement(valeur->getBrancheId(), valeur->getValeurId(), valeur->getRevision()));
					}

					// Le server confirme la révision de la valeur qu'il a reçu
					else if(ConfirmValeurChangement* chgt = dynamic_cast<ConfirmValeurChangement*>(*itCh)) {
						Valeur* valeur = getValeur(_serverTreeProxy, chgt->getBrancheId(), chgt->getValeurId());

						MarqueurDistant* marqueur = valeur->getMarqueur(_serverTreeProxy);
						marqueur->setConfirmedRevision(chgt->getRevision());
					}

					// Le server confirme la révision de la branche qu'il a reçu
					else if(ConfirmBrancheChangement* chgt = dynamic_cast<ConfirmBrancheChangement*>(*itCh)) {
						Branche* branche = getBranche(_serverTreeProxy, chgt->getBrancheId());

						MarqueurDistant* marqueur = branche->getMarqueur(_serverTreeProxy);
						marqueur->setConfirmedRevision(chgt->getRevision());
					}

					else {
						throw DataCommunicationException("Changement inattendu : " + (*itCh)->toString());
					}
				}
				catch(const NotExistingBrancheException& exception) {
					cerr << endl << __FILE__ << ":" << __LINE__ << " Exception : NotExistingBrancheException";
				}
				catch(const DataCommunicationException& exception) {
					cerr << endl << __FILE__ << ":" << __LINE__ << exception.what();
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
		cerr << endl << __FILE__ << ":" << __LINE__ << " Exception : " << exception.what();
	}
}

void ClientDataTree::sendChangementsToServer(vector<Changement*>& changements) {
	vector<Changement*>::iterator iter;
	Interlocutor2* interlocutor = _serverTreeProxy->getInterlocutor();

	if(changements.size()) {
		for(iter = changements.begin() ; iter != changements.end() ; iter++) {
			LOGINFO(("'%s' to '%s' : '%s'", _clientName.c_str(), interlocutor->getName().c_str(), (*iter)->toString().c_str()));
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
