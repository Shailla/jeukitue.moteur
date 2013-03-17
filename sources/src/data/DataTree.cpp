/*
 * DataTree.cpp
 *
 *  Created on: 15 mars 2013
 *      Author: Erwin
 */

#include "data/DataTree.h"

DataTree::DataTree() : _root(NULL, 0, "root") {

}

DataTree::~DataTree() {
}

Branche* DataTree::getBranche(const vector<int>& brancheId) throw(NotExistingBrancheException) {
	vector<int>::const_iterator iter;

	Branche* branche = &_root;

	for(iter = brancheId.begin() ; (iter != brancheId.end() && branche != NULL) ; iter++) {
		branche = branche->getSubBranche(*iter);
	}

	if(branche == NULL) {
		throw NotExistingBrancheException();
	}

	return branche;
}

Valeur* DataTree::getValeur(const vector<int>& valeurId) throw(NotExistingValeurException, NotExistingBrancheException) {
	vector<int>::const_iterator iter;

	Branche* branche = &_root;

	for(iter = valeurId.begin() ; (iter != valeurId.end() && branche != NULL) ; iter++) {
		branche = branche->getSubBranche(*iter);
	}

	if(branche == NULL) {
		throw NotExistingBrancheException();
	}

	Valeur* valeur;

	if(iter != valeurId.end())  {
		valeur = branche->getValeur(*iter);
	}
	else {
		throw NotExistingValeurException();
	}

	return valeur;
}

Branche& DataTree::getRoot() {
	return _root;
}
