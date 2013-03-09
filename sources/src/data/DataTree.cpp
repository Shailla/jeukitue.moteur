/*
 * DataTree.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#include <iostream>
#include <vector>

using namespace std;

#include "data/MarqueurClient.h"
#include "data/exception/NotExistingBrancheException.h"

#include "data/DataTree.h"

DataTree::DataTree() : _root(0, "root") {
}

DataTree::~DataTree() {
}

Branche* DataTree::getBranche(vector<int> brancheId) throw(NotExistingBrancheException) {
	vector<int>::iterator iter;

	Branche* branche = &_root;

	cout << "getBranche() : " << endl;

	for(iter = brancheId.begin() ; (iter != brancheId.end() && branche != NULL) ; iter++) {
		cout << *iter << " => ";
		branche = branche->getSubBranche(*iter);

		if(branche == NULL) {
			throw NotExistingBrancheException();
		}
	}

	return branche;
}

Branche& DataTree::getRoot() {
	return _root;
}

Branche* DataTree::addBranche(vector<int>& parentBrancheId, const string& brancheName) {
	return addBrancheForClient(parentBrancheId, brancheName, 0, NULL);
}

Branche* DataTree::addBrancheForClient(vector<int>& parentBrancheId, const string& brancheName, int brancheClientTmpId, Client* client) {
	Branche* parentBranche = getBranche(parentBrancheId);

	Branche* branche = parentBranche->createSubBranche(brancheName);

	vector<Client*>::iterator clIter;

	for(clIter = _clients.begin() ; clIter != _clients.end() ; clIter++) {
		Client* cl = *clIter;

		if(cl == client) {
			client->addMarqueur(branche, brancheClientTmpId, false);
		}
		else {
			client->addMarqueur(branche, 0, false);
		}
	}

	return branche;
}

Valeur* DataTree::addValeurInt(vector<int>& parentBrancheId, const string& valeurName, int valeur) {
	return addValeurIntForClient(parentBrancheId, valeurName, 0, valeur, NULL);
}

Valeur* DataTree::addValeurIntForClient(vector<int>& parentBrancheId, const string& valeurName, int valeurClientTmpId, int valeur, Client* client) {
	Branche* parentBranche = getBranche(parentBrancheId);

	Valeur* val = parentBranche->createValeurInt(valeurName, valeur);

	vector<Client*>::iterator clIter;

	for(clIter = _clients.begin() ; clIter != _clients.end() ; clIter++) {
		Client* cl = *clIter;

		if(cl == client) {
			client->addMarqueur(val, valeurClientTmpId, false);
		}
		else {
			client->addMarqueur(val, 0, false);
		}
	}

	return val;
}

void DataTree::addClient(const string& clientName) {
	Client* client = new Client(clientName);
	_clients.push_back(client);
}

vector<Client*>& DataTree::getClients() {
	return _clients;
}


void DataTree::diffuseChangements(void) {
	vector<Client*>::iterator clientIter;

	for(clientIter = _clients.begin() ; clientIter != _clients.end() ; clientIter++) {
		Client* client = *clientIter;
		map<Donnee*, MarqueurClient*>& marqueurs = client->getMarqueurs();

		map<Donnee*, MarqueurClient*>::iterator marqIter;

		for(marqIter = marqueurs.begin() ; marqIter != marqueurs.end() ; marqIter++) {
			MarqueurClient* marqueur = marqIter->second;

			if(!marqueur->isUpToDate()) {
				//collecte(marqueur);
			}
		}
	}
}