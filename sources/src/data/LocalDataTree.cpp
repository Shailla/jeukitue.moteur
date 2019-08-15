/*
 * LocalDataTree.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

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

#include "data/LocalDataTree.h"

using namespace std;
using namespace jkt;

LocalDataTree::LocalDataTree() : DataTree(TREE_LOCAL) {
}

LocalDataTree::~LocalDataTree() {
}

Branche* LocalDataTree::createBranche(DistantTreeProxy* distant, const vector<int>& parentBrancheId, const string& brancheName) {
	Branche* parentBranche = getBrancheFromDistant(0, parentBrancheId);

	Branche* branche = parentBranche->createSubBrancheForServer(0, brancheName, DONNEE_LOCAL, 0);

	_donnees.insert(branche);

	return branche;
}

Branche* LocalDataTree::createPrivateBranche(const vector<int>& parentBrancheId, const string& brancheName) {
	Branche* parentBranche = getBrancheFromDistant(0, parentBrancheId);

	Branche* branche = parentBranche->createSubBrancheForServer(0, brancheName, DONNEE_LOCAL, 0);

	_donnees.insert(branche);

	return branche;
}

Branche* LocalDataTree::getBrancheFromDistant(DistantTreeProxy* distant, const vector<int>& brancheId) {
	vector<int>::const_iterator iter;

	Branche* branche = &getRoot();

	for(iter = brancheId.begin() ; (iter != brancheId.end() && branche != NULL) ; iter++) {
		int id = *iter;
		branche = branche->getSubBrancheByIdOrDistantTmpId(0, id);
	}

	if(branche == NULL) {
		throw NotExistingBrancheException("LocalDataTree::getBrancheFromDistant");
	}

	return branche;
}

Valeur* LocalDataTree::getValeurFromDistant(DistantTreeProxy* distant, const vector<int>& brancheId, int valeurId) {
	vector<int>::const_iterator iter;

	Branche* branche = &getRoot();

	if(brancheId.size() > 0) {
		for(iter = brancheId.begin() ; (iter != brancheId.end() && branche != NULL) ; iter++) {
			branche = branche->getSubBrancheByIdOrDistantTmpId(0, *iter);
		}

		if(branche == NULL) {
			throw NotExistingBrancheException("LocalDataTree::getValeurFromDistant 1");
		}
	}

	Valeur* valeur = branche->getValeurByIdOrTmpId(0, valeurId);

	if(!valeur) {
		throw NotExistingValeurException("LocalDataTree::getValeurFromDistant 2");
	}

	return valeur;
}

Valeur* LocalDataTree::createValeur(DistantTreeProxy* distant, UPDATE_MODE updateMode, const vector<int>& parentBrancheId, const string& valeurName, const AnyData valeur) {
	Branche* parentBranche = getBrancheFromDistant(0, parentBrancheId);

	Valeur* val = parentBranche->createValeurForServeur(0, updateMode, valeurName, 0, valeur);

	_donnees.insert(val);

	return val;
}

void LocalDataTree::initDistantBranche(DistantTreeProxy* distant, Branche* branche) {
	// This function must never be called
}
