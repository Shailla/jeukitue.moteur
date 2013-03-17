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
#include "data/communication/DataSerializer.h"

#include "data/ServeurDataTree.h"

ServeurDataTree::ServeurDataTree() : _root(NULL, 0, "root") {
}

ServeurDataTree::~ServeurDataTree() {
}

Branche* ServeurDataTree::addBranche(vector<int>& parentBrancheId, const string& brancheName) {
	return addBrancheFromDistant(parentBrancheId, brancheName, 0, NULL);
}

Branche* ServeurDataTree::addBrancheFromDistant(vector<int>& parentBrancheId, const string& brancheName, int brancheClientTmpId, Client* client) {
	Branche* parentBranche = getBranche(parentBrancheId);

	Branche* branche = parentBranche->createSubBranche(brancheName);

	return (Branche*)addMarqueurFromDistant(client, branche, brancheClientTmpId);
}

Valeur* ServeurDataTree::addValeurInt(vector<int>& parentBrancheId, const string& valeurName, int valeur) {
	return addValeurIntFromDistant(parentBrancheId, valeurName, 0, valeur, NULL);
}

Valeur* ServeurDataTree::addValeurIntFromDistant(vector<int>& parentBrancheId, const string& valeurName, int valeurClientTmpId, int valeur, Client* client) {
	Branche* parentBranche = getBranche(parentBrancheId);

	Valeur* val = parentBranche->createValeurInt(valeurName, valeur);

	return (Valeur*)addMarqueurFromDistant(client, val, valeurClientTmpId);
}

Donnee* ServeurDataTree::addMarqueurFromDistant(Client* client, Donnee* donnee, int donneeClientTmpId) {
	vector<Client*>::iterator clIter;

	for(clIter = _clients.begin() ; clIter != _clients.end() ; clIter++) {
		Client* cl = *clIter;

		if(cl == client) {
			cl->addMarqueur(donnee, donneeClientTmpId);
		}
		else {
			cl->addMarqueur(donnee, 0);
		}
	}

	return donnee;
}

Client* ServeurDataTree::addDistant(const string& clientName) {
	Client* client = new Client(clientName);

	// Init the marqueurs
	initDistantBranche(client, &_root);

	_clients.push_back(client);

	return client;
}

void ServeurDataTree::initDistantBranche(Client* client, Branche* branche) {
	MarqueurClient* marqueur = client->addMarqueur(branche, 0);

	if(branche == &_root) {	// Do not add the root branche to the distants, because it's a default existing element
		marqueur->setSentRevision(0);
	}

	// Init sub-branches
	{
		map<int, Branche*>& subBranches = branche->getSubBranches();
		map<int, Branche*>::iterator iterBr;

		for(iterBr = subBranches.begin() ; iterBr != subBranches.end() ; iterBr++) {
			initDistantBranche(client, iterBr->second);
		}
	}

	// Init values
	{
		map<int, Valeur*>& valeurs = branche->getValeurs();
		map<int, Valeur*>::iterator iterVl;

		for(iterVl = valeurs.begin() ; iterVl != valeurs.end() ; iterVl++) {
			client->addMarqueur(iterVl->second, 0);
		}
	}
}

vector<Client*>& ServeurDataTree::getDistants() {
	return _clients;
}


void ServeurDataTree::diffuseChangements(void) {
	vector<Client*>::iterator clientIter;
	vector<Changement*> changements;

	for(clientIter = _clients.begin() ; clientIter != _clients.end() ; clientIter++) {
		Client* client = *clientIter;
		client->collecteChangements(changements);

		if(changements.size()) {
			ostringstream out;
			DataSerializer::toStream(changements, out);
			client->sendData(out);
		}
	}
}
