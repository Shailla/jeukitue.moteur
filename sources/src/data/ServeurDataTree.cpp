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

#include "util/CollectionsUtils.h"
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

ServeurDataTree::ServeurDataTree() : DataTree(TREE_SERVER) {
}

ServeurDataTree::~ServeurDataTree() {
}

Branche* ServeurDataTree::createBranche(DistantTreeProxy* distant, const vector<int>& parentBrancheId, const string& brancheName) {
	return addBrancheFromDistant(parentBrancheId, brancheName, 0, 0, distant);
}

Branche* ServeurDataTree::createPrivateBranche(const vector<int>& parentBrancheId, const string& brancheName) {
	return addPrivateBrancheFromDistant(parentBrancheId, brancheName, 0, 0, NULL);
}

Branche* ServeurDataTree::addBrancheFromDistant(const vector<int>& parentBrancheId, const string& brancheName, int brancheClientTmpId, int revision, DistantTreeProxy* distant) {
	Branche* parentBranche = getBrancheFromDistant(distant, parentBrancheId);

	AbstractBranche* branche = parentBranche->createSubBrancheForServer(distant, brancheName, DONNEE_DEFAULT, revision);

	return (Branche*)initDonneeAndMarqueurFromDistant(distant, branche, brancheClientTmpId);
}

PrivateBranche* ServeurDataTree::addPrivateBrancheFromDistant(const vector<int>& parentBrancheId, const string& brancheName, int brancheClientTmpId, int revision, DistantTreeProxy* distant) {
	Branche* parentBranche = getBrancheFromDistant(distant, parentBrancheId);

	AbstractBranche* branche = parentBranche->createSubBrancheForServer(distant, brancheName, DONNEE_PRIVATE, revision);

	return (PrivateBranche*)initDonneeAndMarqueurFromDistant(distant, branche, brancheClientTmpId);
}

Valeur* ServeurDataTree::createValeur(DistantTreeProxy* distant, UPDATE_MODE updateMode, const vector<int>& parentBrancheId, const string& valeurName, const AnyData valeur) {
	return addValeurFromDistant(parentBrancheId, updateMode, valeurName, 0, 0, valeur, distant);
}

Valeur* ServeurDataTree::addValeurFromDistant(const vector<int>& parentBrancheId, UPDATE_MODE updateMode, const string& valeurName, int valeurClientTmpId, int revision, const AnyData valeur, DistantTreeProxy* distant) {
	Branche* parentBranche = getBrancheFromDistant(distant, parentBrancheId);

	Valeur* val = parentBranche->createValeurForServeur(distant, updateMode, valeurName, revision, valeur);

	return (Valeur*)initDonneeAndMarqueurFromDistant(distant, val, valeurClientTmpId);
}

Donnee* ServeurDataTree::initDonneeAndMarqueurFromDistant(DistantTreeProxy* client, Donnee* donnee, int donneeClientTmpId) {
	_donnees.insert(donnee);

	DONNEE_TYPE type = donnee->getDonneeType();

	switch(type) {
	case DONNEE_PUBLIC:
	case DONNEE_PRIVATE:
	{
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
	}
	break;
	case DONNEE_PRIVATE_SUB:
		if(!client) {
			cerr << endl << "Initialisation de donnée impossible sans client spécifié sur branche privée";
		}
		else if(contains<DistantTreeProxy*>(_clients, client)) {
			client->addMarqueur(donnee, donneeClientTmpId);
		}
		else {
			cerr << endl << "Initialisation de donnée impossible car client inconnu sur branche privée";
		}

		break;
	case DONNEE_LOCAL:
		// Pas besoin d'initialiser les marqueurs en mode local ils ne serviront à rien
		break;
	default:
		cerr << endl << "Donnée à initialiser de type inconnu";
		break;
	}

	return donnee;
}

DistantTreeProxy* ServeurDataTree::addDistant(Interlocutor2* interlocutor) {
	// Init the marqueurs
	DistantTreeProxy* distant = new DistantTreeProxy(interlocutor);
	initDistantBranche(distant, &getRoot());

	LOGINFO(("AJOUT D'UN DISTANT : '%s'", interlocutor->getName().c_str()));

	_clients.push_back(distant);

	return distant;
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

	Branche* bra = dynamic_cast<Branche*>(branche);

	if(bra) {
		// Init sub-branches
		{
			vector<Branche*>& subBranches = bra->getSubBranches(0);
			vector<Branche*>::iterator itBr;

			for(itBr = subBranches.begin() ; itBr != subBranches.end() ; itBr++) {
				initDistantBranche(distant, *itBr);
			}
		}

		// Init values
		{
			vector<Valeur*>& valeurs = bra->getValeurs(0);
			vector<Valeur*>::iterator iterVl;

			for(iterVl = valeurs.begin() ; iterVl != valeurs.end() ; iterVl++) {
				distant->addMarqueur(*iterVl, 0);
			}
		}
	}
	else {
		PrivateBranche* pr = static_cast<PrivateBranche*>(branche);
		map<DistantTreeProxy*, PrivateBranche::DistantPrivateBranche>::iterator dt;

		for(dt = pr->getDistants().begin() ; dt != pr->getDistants().end() ; dt++) {
			// Init sub-branches
			{
				vector<Branche*>& subBranches = pr->getSubBranches(dt->first);
				vector<Branche*>::iterator itBr;

				for(itBr = subBranches.begin() ; itBr != subBranches.end() ; itBr++) {
					initDistantBranche(distant, *itBr);
				}
			}

			// Init values
			{
				vector<Valeur*>& valeurs = pr->getValeurs(dt->first);
				vector<Valeur*>::iterator iterVl;

				for(iterVl = valeurs.begin() ; iterVl != valeurs.end() ; iterVl++) {
					distant->addMarqueur(*iterVl, 0);
				}
			}
		}
	}
}

void ServeurDataTree::diffuseChangementsToClients(void) {
	try {
		vector<DistantTreeProxy*>::const_iterator clientIter;
		vector<Changement*> changements;

		Uint32 now = SDL_GetTicks();

		DistantTreeProxy* client;
		Interlocutor2* interlocutor;
		vector<Changement*>::iterator itCh;

		Uint32 time;
		saveTime(time);

		for(clientIter = getDistants().begin() ; clientIter != getDistants().end() ; clientIter++) {
			client = *clientIter;

			if(now - client->getUpdateServerToClientTime() >= 200) {
				client->setUpdateServerToClientTime(now);

				interlocutor = client->getInterlocutor();
				client->collecteChangementsInServerTree(changements);

				// Priorise et filtre les changements qui seront vraiment envoyés

				// Emission des changements
				if(changements.size()) {
//					for(itCh = changements.begin() ; itCh != changements.end() ; ++itCh) {
//						LOGINFO(("serveur to '%s' : '%s'", interlocutor->getName().c_str(), (*itCh)->toString().c_str()));
//					}

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
	}
	catch(NotExistingBrancheException& exception) {
		LOGERROR(("Unmanaged NotExistingBrancheException : %s", exception.what()));
	}
	catch(NotExistingValeurException& exception) {
		LOGERROR(("Unmanaged NotExistingValeurException : %s", exception.what()));
	}
}

Branche* ServeurDataTree::getBrancheByDistantTmpId(DistantTreeProxy* distant, const vector<int>& parentBrancheId, int brancheTmpId) throw(NotExistingBrancheException) {
	vector<int>::const_iterator iter;

	AbstractBranche* parentBranche = &getRoot();

	for(iter = parentBrancheId.begin() ; (iter != parentBrancheId.end() && parentBranche != NULL) ; iter++) {
		int aa = *iter;
		parentBranche = parentBranche->getSubBrancheByIdOrDistantTmpId(distant, aa);
	}

	if(!parentBranche) {
		throw NotExistingBrancheException("ServeurDataTree::getBrancheByDistantTmpId 1");
	}

	Branche* branche = parentBranche->getSubBrancheByDistantTmpId(distant, brancheTmpId);

	if(!branche) {
		throw NotExistingBrancheException("ServeurDataTree::getBrancheByDistantTmpId 2");
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
		throw NotExistingBrancheException("ServeurDataTree::getValeurByDistantTmpId 1");
	}

	Valeur* valeur = branche->getValeurByDistantTmpId(distant, valeurTmpId);

	if(!valeur) {
		throw NotExistingValeurException("ServeurDataTree::getValeurByDistantTmpId 2");
	}

	return valeur;
}

void ServeurDataTree::receiveChangementsFromClients() {
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

				LOGINFO(("NOMBRE DE CHANGEMENTS A PRENDRE EN COMPTE : %d", changements.size()));


				for(itCh = changements.begin() ; itCh != changements.end() ; itCh++) {
					try {
						LOGINFO(("serveur from '%s' : '%s'", interlocutor->getName().c_str(), (*itCh)->toString().c_str()));

						// Le client confirme la révision de branche qu'il a reçu
						if(ConfirmBrancheChangement* chgt = dynamic_cast<ConfirmBrancheChangement*>(*itCh)) {
							Branche* branche = getBranche(distant, chgt->getBrancheId());
							MarqueurDistant* marqueur = distant->getMarqueur(branche);
							marqueur->setConfirmedRevision(chgt->getRevision());
						}

						// Le client confirme la révision de valeur qu'il a reçu
						else if(ConfirmValeurChangement* chgt = dynamic_cast<ConfirmValeurChangement*>(*itCh)) {
							Valeur* valeur = getValeur(distant, chgt->getBrancheId(), chgt->getValeurId());
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
								cout << endl << "Distant '" << distant->getInterlocutor()->getName() << "' => La branche de parent " << CollectionsUtils::toString(chgt->getParentBrancheId()) << " et d'id temportaire " << chgt->getBrancheTmpId() << " n'existe pas encore, on va la créer" << flush;
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
								valeur = addValeurFromDistant(chgt->getParentBrancheId(), chgt->getUpdateMode(), chgt->getValeurName(), chgt->getValeurTmpId(), chgt->getRevision(), chgt->getValeur(), distant);
							}

							MarqueurDistant* marqueur = distant->getMarqueur(valeur);
							marqueur->setConfirmedRevision(valeur->getRevision());

							answers.push_back(new AcceptAddValeurFromClientChangement(valeur->getBrancheId(), chgt->getValeurTmpId(), valeur->getValeurId(), valeur->getRevision()));
						}

						// Le client demande la modification d'une valeur
						else if(UpdateValeurFromClientChangement* chgt = dynamic_cast<UpdateValeurFromClientChangement*>(*itCh)) {
							Valeur* valeur = getValeur(distant, chgt->getParentBrancheId(), chgt->getValeurId());

							if(valeur->getRevision() < chgt->getRevision()) {
								valeur->setValeur(chgt->getRevision(), chgt->getValeur());

								MarqueurDistant* marqueur = distant->getMarqueur(valeur);
								marqueur->setConfirmedRevision(chgt->getRevision());

								answers.push_back(new ConfirmValeurChangement(valeur->getBrancheId(), chgt->getValeurId(), valeur->getRevision()));
							}
							else {
								LOGERROR((" Exception : La révision client est plus ancienne que celle du serveur"));
							}
						}

						// Cas d'erreur
						else {
							throw DataCommunicationException("Changement de type inconnu");
						}
					}
					catch(const NotExistingValeurException& exception) {
						LOGERROR(("NotExistingValeurException"));
					}
					catch(const NotExistingBrancheException& exception) {
						LOGERROR(("NotExistingBrancheException"));
					}
					catch(const DataCommunicationException& exception) {
						LOGERROR(("DataCommunicationException : %s", exception.what()));
					}

					delete *itCh;
				}

				// Emission des changements au client
				if(answers.size()) {
					vector<Changement*>::iterator itCh;

					for(itCh = answers.begin() ; itCh != answers.end() ; itCh++) {
						LOGINFO(("serveur '%s', : '%s'", interlocutor->getName().c_str(), (*itCh)->toString().c_str()));
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
		LOGERROR(("DataCommunicationException : %s", exception.what()));
	}
	catch(NotExistingBrancheException& exception) {
		LOGERROR(("Unmanaged NotExistingBrancheException : %s", exception.what()));
	}
	catch(NotExistingValeurException& exception) {
		LOGERROR(("Unmanaged NotExistingValeurException : %s", exception.what()));
	}
}

