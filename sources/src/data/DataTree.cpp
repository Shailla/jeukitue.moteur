/*
 * DataTree.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#include <vector>

using namespace std;

#include "data/DataTree.h"
#include "data/MarqueurBrancheClient.h"

DataTree::DataTree() : _root(0) {
}

DataTree::~DataTree() {
}

Branche* DataTree::getBranche(vector<int> brancheId) {
	vector<int>::iterator iter;

	Branche* branche = &_root;

	for(iter = brancheId.begin() ; (iter != brancheId.end() && branche != 0) ; iter++) {
		branche = branche->getSubBranche(*iter);
	}

	return branche;
}

Branche& DataTree::getRoot() {
	return _root;
}

Branche* DataTree::addBranche(vector<int> parentBrancheId) {
	return addBrancheForClient(parentBrancheId, 0, NULL);
}

Branche* DataTree::addBrancheForClient(vector<int> parentBrancheId, int brancheClientTmpId, Client* client) {
	Branche* branche = addBranche(parentBrancheId);

	vector<Client>::iterator clIter;

	for(clIter = _clients.begin() ; clIter != _clients.end() ; clIter++) {
		Client& cl = *clIter;

		MarqueurBrancheClient* marqueur = new MarqueurBrancheClient(branche);

		if(&cl == client) {
			marqueur->setTemporaryId(brancheClientTmpId);
		}

		client->addMarqueur(*marqueur);
	}

	return branche;
}

void DataTree::diffuseChangements(void) {
	vector<Client>::iterator clientIter;

	for(clientIter = _clients.begin() ; clientIter != _clients.end() ; clientIter++) {
		Client& client = *clientIter;
		vector<MarqueurClient>& marqueurs = client.getMarqueurs();

		vector<MarqueurClient>::iterator marqIter;

		for(marqIter = marqueurs.begin() ; marqIter != marqueurs.end() ; marqIter++) {
			MarqueurClient& marqueur = *marqIter;

			if(!marqueur.isUpToDate()) {
				//collecte(marqueur);
			}
		}
	}
}
