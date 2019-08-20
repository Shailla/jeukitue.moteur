/*
 * DataTree.cpp
 *
 *  Created on: 15 mars 2013
 *      Author: Erwin
 */

#include <iostream>

#include "data/exception/NotExistingBrancheException.h"
#include "data/exception/NotExistingValeurException.h"

#include "data/DataTree.h"

using namespace std;

DataTree::DataTree(TREE_TYPE treeType) : _root(this, 0, DONNEE_PUBLIC, 0, -1) {
	_treeType = treeType;
}

DataTree::~DataTree() {
}

Branche* DataTree::getBranche(DistantTreeProxy* distant, const vector<int>& brancheId) {
	vector<int>::const_iterator iter;

	Branche* branche = &_root;

	for(iter = brancheId.begin() ; (iter != brancheId.end() && branche != NULL) ; iter++) {
		int id = *iter;
		branche = branche->getSubBrancheByIdOrTmpId(distant, id);
	}

	if(branche == NULL) {
		throw NotExistingBrancheException("DataTree::getBranche 1");
	}

	return branche;
}

Branche* DataTree::getBranche(DistantTreeProxy* distant, const vector<string>& branchePath) {
	vector<string>::const_iterator iter;

	Branche* branche = &_root;

	for(iter = branchePath.begin() ; (iter != branchePath.end() && branche != NULL) ; iter++) {
		branche = branche->getSubBrancheByName(distant, *iter);
	}

	if(branche == NULL) {
		throw NotExistingBrancheException("DataTree::getBranche 2");
	}

	return branche;
}

Valeur* DataTree::getValeur(DistantTreeProxy* distant, const vector<int>& brancheId, int valeurId) {
	vector<int>::const_iterator iter;

	Branche* branche = &_root;

	if(brancheId.size() > 0) {
		for(iter = brancheId.begin() ; (iter != brancheId.end() && branche != NULL) ; iter++) {
			branche = branche->getSubBrancheByIdOrTmpId(distant, *iter);
		}

		if(branche == NULL) {
			throw NotExistingBrancheException("DataTree::getValeur 1");
		}
	}

	Valeur* valeur = branche->getValeurByIdOrTmpId(distant, valeurId);

	if(!valeur) {
		throw NotExistingValeurException("DataTree::getValeur 2");
	}

	return valeur;
}

Branche& DataTree::getRoot() {
	return _root;
}

bool DataTree::containsDonnee(Donnee* donnee) {
	return _donnees.find(donnee) != _donnees.end();
}
