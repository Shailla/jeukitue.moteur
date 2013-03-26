/*
 * ServeurDataTree.cpp
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
#include "util/types/IntData.h"
#include "data/communication/DataSerializer.h"
#include "data/communication/message/ClientToServer/AddBrancheFromClientChangement.h"
#include "data/communication/message/ClientToServer/AddValeurFromClientChangement.h"
#include "data/communication/message/ServerToClient/AcceptAddBrancheFromClientChangement.h"
#include "data/communication/message/ConfirmBrancheChangement.h"
#include "data/communication/message/ConfirmValeurChangement.h"

#include "data/ServeurDataTree.h"

using namespace JktUtils;

ServeurDataTree::ServeurDataTree() {
}

ServeurDataTree::~ServeurDataTree() {
}

Branche* ServeurDataTree::createBranche(const vector<int>& parentBrancheId, const string& brancheName) {
	return addBrancheFromDistant(parentBrancheId, brancheName, 0, NULL);
}

Branche* ServeurDataTree::addBrancheFromDistant(const vector<int>& parentBrancheId, const string& brancheName, int brancheClientTmpId, Distant* distant) {
	Branche* parentBranche = getBranche(parentBrancheId);

	Branche* branche = parentBranche->createSubBrancheForServer(brancheName);

	return (Branche*)addMarqueurFromDistant(distant, branche, brancheClientTmpId);
}

Valeur* ServeurDataTree::createValeurInt(const vector<int>& parentBrancheId, const string& valeurName, int valeur) {
	return addValeurIntFromDistant(parentBrancheId, valeurName, 0, valeur, NULL);
}

Valeur* ServeurDataTree::addValeurIntFromDistant(const vector<int>& parentBrancheId, const string& valeurName, int valeurClientTmpId, int valeur, Distant* distant) {
	Branche* parentBranche = getBranche(parentBrancheId);

	Valeur* val = parentBranche->createValeurIntForServeur(valeurName, valeur);

	return (Valeur*)addMarqueurFromDistant(distant, val, valeurClientTmpId);
}

Donnee* ServeurDataTree::addMarqueurFromDistant(Distant* client, Donnee* donnee, int donneeClientTmpId) {
	vector<Distant*>::iterator clIter;

	// Ajoute un marqueur pour chaque distant et met l'identifiant temporaire dans le distant du client qui a créé la donnée
	for(clIter = getDistants().begin() ; clIter != getDistants().end() ; clIter++) {
		Distant* cl = *clIter;

		if(cl == client) {
			cl->addMarqueur(donnee, donneeClientTmpId);
		}
		else {
			cl->addMarqueur(donnee, 0);
		}
	}

	return donnee;
}

Distant* ServeurDataTree::addDistant(const string& distantName) {
	Distant* distant = new Distant(distantName);

	// Init the marqueurs
	initDistantBranche(distant, &getRoot());

	_clients.push_back(distant);

	return distant;
}

vector<Distant*>& ServeurDataTree::getDistants() {
	return _clients;
}

void ServeurDataTree::initDistantBranche(Distant* distant, Branche* branche) {
	MarqueurDistant* marqueur = distant->addMarqueur(branche, 0);

	if(branche == &getRoot()) {	// Do not add the root branche to the distants, because it's a default existing element
		marqueur->setSentRevision(0);
		marqueur->setConfirmedRevision(0);
	}

	// Init sub-branches
	{
		map<int, Branche*>& subBranches = branche->getSubBranches();
		map<int, Branche*>::iterator itBr;

		for(itBr = subBranches.begin() ; itBr != subBranches.end() ; itBr++) {
			initDistantBranche(distant, itBr->second);
		}
	}

	// Init values
	{
		map<int, Valeur*>& valeurs = branche->getValeurs();
		map<int, Valeur*>::iterator iterVl;

		for(iterVl = valeurs.begin() ; iterVl != valeurs.end() ; iterVl++) {
			distant->addMarqueur(iterVl->second, 0);
		}
	}
}

void ServeurDataTree::diffuseChangementsToClients(void) {
	vector<Distant*>::iterator clientIter;
	vector<Changement*> changements;

	for(clientIter = getDistants().begin() ; clientIter != getDistants().end() ; clientIter++) {
		Distant* client = *clientIter;
		client->collecteChangementsInServerTree(changements);

		// Emission des changements
		if(changements.size()) {
			vector<Changement*>::iterator itCh;

			for(itCh = changements.begin() ; itCh != changements.end() ; itCh++) {
				cout << endl << "serveur" << " to " << client->getDebugName() << "\t : " << (*itCh)->toString() << flush;
			}

			ostringstream out;
			DataSerializer::toStream(changements, out);
			client->setDataToSend(new string(out.str()));
		}
	}
}

Branche* ServeurDataTree::getBrancheByDistantTmpId(Distant* distant, const vector<int>& parentBrancheId, int brancheTmpId) throw(NotExistingBrancheException) {
	vector<int>::const_iterator iter;

	Branche* parentBranche = &getRoot();

	for(iter = parentBrancheId.begin() ; (iter != parentBrancheId.end() && parentBranche != NULL) ; iter++) {
		parentBranche = parentBranche->getSubBranche(*iter);
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

void ServeurDataTree::receiveChangementsFromClient() {
	try {
		vector<Distant*>::iterator it;

		for(it = _clients.begin() ; it != _clients.end() ; it++) {
			Distant* distant = *it;
			string* data = distant->getDataReceived();

			if(data) {
				vector<Changement*> changements;
				vector<Changement*> answers;

				istringstream in(*data);
				DataSerializer::fromStream(changements, in);

				vector<Changement*>::iterator itCh;

				try {
					for(itCh = changements.begin() ; itCh != changements.end() ; itCh++) {
						cout << endl << "serveur" << " from " << distant->getDebugName() << "\t : " << (*itCh)->toString() << flush;

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
								branche = addBrancheFromDistant(chgt->getParentBrancheId(), chgt->getBrancheName(), chgt->getBrancheTmpId(), distant);
							}

							MarqueurDistant* marqueur = distant->getMarqueur(branche);
							marqueur->setConfirmedRevision(branche->getRevision());
							answers.push_back(new AcceptAddBrancheFromClientChangement(branche->getParentBrancheId(), chgt->getBrancheTmpId(), branche->getBrancheId(), branche->getRevision()));
						}

						// Le client demande la création d'une nouvelle valeur
						else if(AddValeurFromClientChangement* chgt = dynamic_cast<AddValeurFromClientChangement*>(*itCh)) {
							if(IntData* value = dynamic_cast<IntData*>(chgt->getValeur())) {
								addValeurIntFromDistant(chgt->getBrancheId(), chgt->getValeurName(), chgt->getValeurTmpId(), value->getValue(), distant);
							}
							else {
								throw DataCommunicationException("Valeur de type inconnu");
							}
						}

						// Cas d'erreur
						else {
							throw DataCommunicationException("Changement de type inconnu");
						}
					}
				}
				catch(const NotExistingValeurException& exception) {
					cerr << endl << __FILE__ << ":" << __LINE__ << " Exception : NotExistingValeurException";
				}
				catch(const NotExistingBrancheException& exception) {
					cerr << endl << __FILE__ << ":" << __LINE__ << " Exception : NotExistingBrancheException";
				}
				catch(const DataCommunicationException& exception) {
					cerr << endl << __FILE__ << ":" << __LINE__ << " Exception : " << exception.getMessage();
				}

				delete data;

				// Emission des changements au client
				if(answers.size()) {
					vector<Changement*>::iterator itCh;

					for(itCh = answers.begin() ; itCh != answers.end() ; itCh++) {
						cout << endl << "serveur" << " to " << distant->getDebugName() << "\t : " << (*itCh)->toString() << flush;
					}

					ostringstream out;
					DataSerializer::toStream(answers, out);
					distant->setDataToSend(new string(out.str()));
				}
			}
		}
	}
	catch(DataCommunicationException& exception) {
		cerr << endl << __FILE__ << ":" << __LINE__ << " Exception : " << exception.getMessage();
	}
}

