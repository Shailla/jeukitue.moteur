/*
 * DataTree.h
 *
 *  Created on: 15 mars 2013
 *      Author: Erwin
 */

#ifndef DATATREE_H_
#define DATATREE_H_

#include "data/exception/NotExistingBrancheException.h"
#include "data/exception/NotExistingValeurException.h"
#include "data/Branche.h"
#include "data/Client.h"

class DataTree {
	Branche _root;

public:
	DataTree();
	virtual ~DataTree();


	/* *********************************
	 * Accessors
	 * ********************************/

	/**
	 * Retourne la branche idéntifiée.
	 *
	 * @param brancheId coordonnées de la branche dans l'arbre
	 */
	Branche* getBranche(vector<int>& brancheId) throw(NotExistingBrancheException);
	Valeur* getValeur(vector<int>& valeurId) throw(NotExistingValeurException, NotExistingBrancheException);
	Branche& getRoot();
};

#endif /* DATATREE_H_ */
