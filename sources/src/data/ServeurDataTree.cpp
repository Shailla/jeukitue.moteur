/*
 * ServeurDataTree.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

#include "util/Trace.h"
#include "util/TraceMethod.h"
#include "data/MarqueurDistant.h"
#include "data/exception/NotExistingBrancheException.h"
#include "data/exception/DataCommunicationException.h"
#include "data/communication/DataSerializer.h"
#include "data/communication/message/ClientToServer/AddBrancheFromClientChangement.h"
#include "data/communication/message/ClientToServer/AddValeurFromClientChangement.h"
#include "data/communication/message/ServerToClient/AcceptAddBrancheFromClientChangement.h"
#include "data/communication/message/ServerToClient/AcceptAddValeurFromClientChangement.h"
#include "data/communication/message/ClientToServer/UpdateValeurFromClientChangement.h"
#include "data/communication/message/ConfirmBrancheChangement.h"
#include "data/communication/message/ConfirmValeurChangement.h"

#include "data/ServeurDataTree.h"

using namespace JktUtils;

ServeurDataTree::ServeurDataTree() {
}

ServeurDataTree::~ServeurDataTree() {
}

Branche* ServeurDataTree::createBranche(const vector<int>& parentBrancheId, const string& brancheName) {
	return addBrancheFromDistant(parentBrancheId, brancheName, 0, 0, NULL);
}

Branche* ServeurDataTree::addBrancheFromDistant(const vector<int>& parentBrancheId, const string& brancheName, int brancheClientTmpId, int revision, DistantTreeProxy* distant) {
	Branche* parentBranche = getBrancheFromDistant(distant, parentBrancheId);

	Branche* branche = parentBranche->createSubBrancheForServer(brancheName, revision);

	return (Branche*)initDonneeAndMarqueurFromDistant(distant, branche, brancheClientTmpId);
}

Valeur* ServeurDataTree::createValeur(const vector<int>& parentBrancheId, const string& valeurName, const AnyData valeur) {
	return addValeurFromDistant(parentBrancheId, valeurName, 0, 0, valeur, NULL);
}

Valeur* ServeurDataTree::addValeurFromDistant(const vector<int>& parentBrancheId, const string& valeurName, int valeurClientTmpId, int revision, const AnyData valeur, DistantTreeProxy* distant) {
	Branche* parentBranche = getBrancheFromDistant(distant, parentBrancheId);

	Valeur* val = parentBranche->createValeurForServeur(valeurName, revision, valeur);

	return (Valeur*)initDonneeAndMarqueurFromDistant(distant, val, valeurClientTmpId);
}

Donnee* ServeurDataTree::initDonneeAndMarqueurFromDistant(DistantTreeProxy* client, Donnee* donnee, int donneeClientTmpId) {
	_donnees.insert(donnee);

	vector<DistantTreeProxy*>::const_iterator clIter;

	// Ajoute un marqueur pour chaque distant et met l'identifiant temporaire dans le distant du client qui a créé la donnée
	for(clIter = getDistants().begin() ; clIter != getDistants().end() ; clIter++) {
		DistantTreeProxy* cl = *clIter;

		if(cl == client) {
			cl->addMarqueur(donnee, donneeClientTmpId);
		}
		else {
			cl->addMarqueur(donnee, 0);
		}
	}

	return donnee;
}

void ServeurDataTree::addDistant(Interlocutor2* interlocutor) {
	// Init the marqueurs
	DistantTreeProxy* distant = new DistantTreeProxy(interlocutor);
	initDistantBranche(distant, &getRoot());

	TRACE().info("AJOUT D'UN DISTANT : '%s'", interlocutor->getName().c_str());

	_clients.push_back(distant);
}

const vector<DistantTreeProxy*>& ServeurDataTree::getDistants() {
	return _clients;
}

void ServeurDataTree::initDistantBranche(DistantTreeProxy* distant, Branche* branche) {
	MarqueurDistant* marqueur = distant->addMarqueur(branche, 0);

	if(branche == &getRoot()) {	// Do not add the root branche to the distants, because it's a default existing element
		marqueur->setSentRevision(0);
		marqueur->setConfirmedRevision(0);
	}

	// Init sub-branches
	{
		vector<Branche*>& subBranches = branche->getSubBranches();
		vector<Branche*>::iterator itBr;

		for(itBr = subBranches.begin() ; itBr != subBranches.end() ; itBr++) {
			initDistantBranche(distant, *itBr);
		}
	}

	// Init values
	{
		vector<Valeur*>& valeurs = branche->getValeurs();
		vector<Valeur*>::iterator iterVl;

		for(iterVl = valeurs.begin() ; iterVl != valeurs.end() ; iterVl++) {
			distant->addMarqueur(*iterVl, 0);
		}
	}
}

void ServeurDataTree::diffuseChangementsToClients(void) {
	TRACEMETHOD();

	vector<DistantTreeProxy*>::const_iterator clientIter;
	vector<Changement*> changements;

	DistantTreeProxy* client;
	Interlocutor2* interlocutor;
	vector<Changement*>::iterator itCh;

	for(clientIter = getDistants().begin() ; clientIter != getDistants().end() ; clientIter++) {
		client = *clientIter;
		interlocutor = client->getInterlocutor();
		client->collecteChangementsInServerTree(changements);

		// Emission des changements
		if(changements.size()) {
			for(itCh = changements.begin() ; itCh != changements.end() ; ++itCh) {
				TRACE().debug("serveur to '%s' : '%s'", interlocutor->getName().c_str(), (*itCh)->toString().c_str());
			}

			ostringstream out;
			DataSerializer::toStream(changements, out);

			for(itCh = changements.begin() ; itCh != changements.end() ; ++itCh) {
				delete *itCh;
			}

			interlocutor->pushDataToSend(new JktUtils::Bytes(out.str()));

			changements.clear();
		}
	}
}

Branche* ServeurDataTree::getBrancheByDistantTmpId(DistantTreeProxy* distant, const vector<int>& parentBrancheId, int brancheTmpId) throw(NotExistingBrancheException) {
	vector<int>::const_iterator iter;

	Branche* parentBranche = &getRoot();

	for(iter = parentBrancheId.begin() ; (iter != parentBrancheId.end() && parentBranche != NULL) ; iter++) {
		int aa = *iter;
		parentBranche = parentBranche->getSubBrancheByIdOrDistantTmpId(distant, aa);
	}

	if(!parentBranche) {
		throw NotExistingBrancheException();
	}

	Branche* branche = parentBranche->getSubBrancheByDistantTmpId(distant, brancheTmpId);

	if(!branche) {
		throw NotExistingBrancheException();
	}

	return branche;
}

Valeur* ServeurDataTree::getValeurByDistantTmpId(DistantTreeProxy* distant, const vector<int>& brancheId, int valeurTmpId) throw(NotExistingBrancheException, NotExistingValeurException) {
	vector<int>::const_iterator iter;

	Branche* branche = &getRoot();

	for(iter = brancheId.begin() ; (iter != brancheId.end() && branche != NULL) ; iter++) {
		branche = branche->getSubBrancheByIdOrDistantTmpId(distant, *iter);
	}

	if(!branche) {
		throw NotExistingBrancheException();
	}

	Valeur* valeur = branche->getValeurByDistantTmpId(distant, valeurTmpId);

	if(!valeur) {
		throw NotExistingValeurException();
	}

	return valeur;
}

void ServeurDataTree::receiveChangementsFromClients() {
	TRACEMETHOD();

	try {
		vector<DistantTreeProxy*>::iterator it;

		for(it = _clients.begin() ; it != _clients.end() ; it++) {
			DistantTreeProxy* distant = *it;
			Interlocutor2* interlocutor = distant->getInterlocutor();
			JktUtils::Bytes* data;

			while((data = interlocutor->popDataReceived())) {
				vector<Changement*> changements;
				vector<Changement*> answers;

				string str(data->getBytes(), data->size());
				delete data;

				istringstream in(str);
				DataSerializer::fromStream(changements, in);

				// Tri des changements de manière à les effectuer en commençant par ceux qui sont le plus proche de la racine, puis les second niveau, puis etc...
				std::sort(changements.begin(), changements.end(), Changement::highestPriority);

				vector<Changement*>::iterator itCh;

				TRACE().info("NOMBRE DE CHANGEMENTS A PRENDRE EN COMPTE : %d", changements.size());


				for(itCh = changements.begin() ; itCh != changements.end() ; itCh++) {
					try {
						TRACE().error("serveur from '%s' : '%s'", interlocutor->getName().c_str(), (*itCh)->toString().c_str());

						// Le client confirme la révision de branche qu'il a reçu
						if(ConfirmBrancheChangement* chgt = dynamic_cast<ConfirmBrancheChangement*>(*itCh)) {
							Branche* branche = getBranche(chgt->getBrancheId());
							MarqueurDistant* marqueur = distant->getMarqueur(branche);
							marqueur->setConfirmedRevision(chgt->getRevision());
						}

						// Le client confirme la révision de valeur qu'il a reçu
						else if(ConfirmValeurChangement* chgt = dynamic_cast<ConfirmValeurChangement*>(*itCh)) {
							Valeur* valeur = getValeur(chgt->getBrancheId(), chgt->getValeurId());
							MarqueurDistant* marqueur = distant->getMarqueur(valeur);
							marqueur->setConfirmedRevision(chgt->getRevision());
						}

						// Le client demande la création d'une nouvelle branche
						else if(AddBrancheFromClientChangement* chgt = dynamic_cast<AddBrancheFromClientChangement*>(*itCh)) {
							Branche* branche;

							try {
								// Si la branche existe déjà on renvoie juste la réponse au client
								branche = getBrancheByDistantTmpId(distant, chgt->getParentBrancheId(), chgt->getBrancheTmpId());
							}
							catch(NotExistingBrancheException& exception) {
								// Si la branche n'existe pas encore on la crée
								branche = addBrancheFromDistant(chgt->getParentBrancheId(), chgt->getBrancheName(), chgt->getBrancheTmpId(), chgt->getRevision(), distant);
							}

							MarqueurDistant* marqueur = distant->getMarqueur(branche);
							marqueur->setConfirmedRevision(branche->getRevision());

							answers.push_back(new AcceptAddBrancheFromClientChangement(branche->getParentBrancheId(), chgt->getBrancheTmpId(), branche->getBrancheId(), branche->getRevision()));
						}

						// Le client demande la création d'une nouvelle valeur
						else if(AddValeurFromClientChangement* chgt = dynamic_cast<AddValeurFromClientChangement*>(*itCh)) {
							Valeur* valeur;

							try {
								// Si la valeur existe déjà on renvoie juste la réponse au client
								valeur = getValeurByDistantTmpId(distant, chgt->getParentBrancheId(), chgt->getValeurTmpId());
							}
							catch(NotExistingValeurException& exception) {
								valeur = addValeurFromDistant(chgt->getParentBrancheId(), chgt->getValeurName(), chgt->getValeurTmpId(), chgt->getRevision(), chgt->getValeur(), distant);
							}

							MarqueurDistant* marqueur = distant->getMarqueur(valeur);
							marqueur->setConfirmedRevision(valeur->getRevision());

							answers.push_back(new AcceptAddValeurFromClientChangement(valeur->getBrancheId(), chgt->getValeurTmpId(), valeur->getValeurId(), valeur->getRevision()));
						}

						// Le client demande la modification d'une valeur
						else if(UpdateValeurFromClientChangement* chgt = dynamic_cast<UpdateValeurFromClientChangement*>(*itCh)) {
							Valeur* valeur = getValeur(chgt->getParentBrancheId(), chgt->getValeurId());

							if(valeur->getRevision() < chgt->getRevision()) {
								valeur->setValeur(chgt->getRevision(), chgt->getValeur());

								MarqueurDistant* marqueur = distant->getMarqueur(valeur);
								marqueur->setConfirmedRevision(chgt->getRevision());

								answers.push_back(new ConfirmValeurChangement(valeur->getBrancheId(), chgt->getValeurId(), valeur->getRevision()));
							}
							else {
								TRACE().error(" Exception : La révision client est plus ancienne que celle du serveur");
							}
						}

						// Cas d'erreur
						else {
							throw DataCommunicationException("Changement de type inconnu");
						}
					}
					catch(const NotExistingValeurException& exception) {
						TRACE().error("NotExistingValeurException");
					}
					catch(const NotExistingBrancheException& exception) {
						TRACE().error("NotExistingBrancheException");
					}
					catch(const DataCommunicationException& exception) {
						TRACE().error("DataCommunicationException : %s", exception.getMessage().c_str());
					}

					delete *itCh;
				}

				// Emission des changements au client
				if(answers.size()) {
					vector<Changement*>::iterator itCh;

					for(itCh = answers.begin() ; itCh != answers.end() ; itCh++) {
						TRACE().info("serveur '%s', : '%s'", interlocutor->getName().c_str(), (*itCh)->toString().c_str());
					}

					ostringstream out;
					DataSerializer::toStream(answers, out);

					for(itCh = answers.begin() ; itCh != answers.end() ; itCh++) {
						delete *itCh;
					}

					interlocutor->pushDataToSend(new JktUtils::Bytes(out.str()));
				}
			}
		}
	}
	catch(DataCommunicationException& exception) {
		TRACE().error("DataCommunicationException : %s", exception.getMessage().c_str());
	}
}

