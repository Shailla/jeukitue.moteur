/*
 * DataTree.cpp
 *
 *  Created on: 15 mars 2013
 *      Author: Erwin
 */

#include <iostream>

using namespace std;

#include "data/DataTree.h"

DataTree::DataTree() : _root(NULL, 0, "root", 0, -1) {
}

DataTree::~DataTree() {
}

Branche* DataTree::getBrancheFromDistant(DistantTreeProxy* distant, const vector<int>& brancheId) throw(NotExistingBrancheException) {
	vector<int>::const_iterator iter;

	Branche* branche = &_root;

	for(iter = brancheId.begin() ; (iter != brancheId.end() && branche != NULL) ; iter++) {
		int id = *iter;
		branche = branche->getSubBrancheByIdOrDistantTmpId(distant, id);
	}

	if(branche == NULL) {
		throw NotExistingBrancheException("DataTree::getBrancheFromDistant");
	}

	return branche;
}

Branche* DataTree::getBranche(const vector<int>& brancheId) throw(NotExistingBrancheException) {
	vector<int>::const_iterator iter;

	Branche* branche = &_root;

	for(iter = brancheId.begin() ; (iter != brancheId.end() && branche != NULL) ; iter++) {
		int id = *iter;
		branche = branche->getSubBrancheByIdOrTmpId(id);
	}

	if(branche == NULL) {
		throw NotExistingBrancheException("DataTree::getBranche 1");
	}

	return branche;
}

Branche* DataTree::getBranche(const vector<string>& branchePath) throw(NotExistingBrancheException) {
	vector<string>::const_iterator iter;

	Branche* branche = &_root;

	for(iter = branchePath.begin() ; (iter != branchePath.end() && branche != NULL) ; iter++) {
		string name = *iter;
		branche = branche->getSubBrancheByName(name);
	}

	if(branche == NULL) {
		throw NotExistingBrancheException("DataTree::getBranche 2");
	}

	return branche;
}

Valeur* DataTree::getValeurFromDistant(DistantTreeProxy* distant, const vector<int>& brancheId, int valeurId) throw(NotExistingValeurException, NotExistingBrancheException) {
	vector<int>::const_iterator iter;

	Branche* branche = &_root;

	if(brancheId.size() > 0) {
		for(iter = brancheId.begin() ; (iter != brancheId.end() && branche != NULL) ; iter++) {
			branche = branche->getSubBrancheByIdOrDistantTmpId(distant, *iter);
		}

		if(branche == NULL) {
			throw NotExistingBrancheException("DataTree::getValeurFromDistant 1");
		}
	}

	Valeur* valeur = branche->getValeurByIdOrTmpId(valeurId);

	if(!valeur) {
		throw NotExistingValeurException("DataTree::getValeurFromDistant 2");
	}

	return valeur;
}

Valeur* DataTree::getValeur(const vector<int>& brancheId, int valeurId) throw(NotExistingValeurException, NotExistingBrancheException) {
	vector<int>::const_iterator iter;

	Branche* branche = &_root;

	if(brancheId.size() > 0) {
		for(iter = brancheId.begin() ; (iter != brancheId.end() && branche != NULL) ; iter++) {
			branche = branche->getSubBrancheByIdOrTmpId(*iter);
		}

		if(branche == NULL) {
			throw NotExistingBrancheException("DataTree::getValeur 1");
		}
	}

	Valeur* valeur = branche->getValeurByIdOrTmpId(valeurId);

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
