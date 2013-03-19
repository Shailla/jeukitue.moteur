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

#include "data/MarqueurDistant.h"
#include "data/exception/NotExistingBrancheException.h"
#include "data/communication/DataSerializer.h"

#include "data/ServeurDataTree.h"

ServeurDataTree::ServeurDataTree() {
}

ServeurDataTree::~ServeurDataTree() {
}

Branche* ServeurDataTree::addBranche(vector<int>& parentBrancheId, const string& brancheName) {
	return addBrancheFromDistant(parentBrancheId, brancheName, 0, NULL);
}

Branche* ServeurDataTree::addBrancheFromDistant(vector<int>& parentBrancheId, const string& brancheName, int brancheClientTmpId, Distant* client) {
	Branche* parentBranche = getBranche(parentBrancheId);

	Branche* branche = parentBranche->createSubBranche(brancheName);

	return (Branche*)addMarqueurFromDistant(client, branche, brancheClientTmpId);
}

Valeur* ServeurDataTree::addValeurInt(vector<int>& parentBrancheId, const string& valeurName, int valeur) {
	return addValeurIntFromDistant(parentBrancheId, valeurName, 0, valeur, NULL);
}

Valeur* ServeurDataTree::addValeurIntFromDistant(vector<int>& parentBrancheId, const string& valeurName, int valeurClientTmpId, int valeur, Distant* client) {
	Branche* parentBranche = getBranche(parentBrancheId);

	Valeur* val = parentBranche->createValeurInt(valeurName, valeur);

	return (Valeur*)addMarqueurFromDistant(client, val, valeurClientTmpId);
}

Donnee* ServeurDataTree::addMarqueurFromDistant(Distant* client, Donnee* donnee, int donneeClientTmpId) {
	vector<Distant*>::iterator clIter;

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

int i = 0;

void ServeurDataTree::initDistantBranche(Distant* distant, Branche* branche) {
	MarqueurDistant* marqueur = distant->addMarqueur(branche, i++);

	if(branche == &getRoot()) {	// Do not add the root branche to the distants, because it's a default existing element
		marqueur->setSentRevision(0);
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

void ServeurDataTree::diffuseChangements(void) {
	vector<Distant*>::iterator clientIter;
	vector<Changement*> changements;

	for(clientIter = getDistants().begin() ; clientIter != getDistants().end() ; clientIter++) {
		Distant* client = *clientIter;
		client->collecteChangements(changements);

		if(changements.size()) {
			ostringstream out;
			DataSerializer::toStream(changements, out);
			client->sendData(out);
		}
	}
}
