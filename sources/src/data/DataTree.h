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
#include "data/Distant.h"

class DataTree {
	Branche _root;
	vector<Distant*> _clients;

public:
	DataTree();
	virtual ~DataTree();

	virtual void initDistantBranche(Distant* distant, Branche* branche) = 0;


	/* *********************************
	 * Accessors
	 * ********************************/

	/**
	 * Retourne la branche idéntifiée.
	 *
	 * @param brancheId coordonnées de la branche dans l'arbre
	 */
	Branche* getBranche(const vector<int>& brancheId) throw(NotExistingBrancheException);
	Valeur* getValeur(const vector<int>& brancheId, int valeurId) throw(NotExistingValeurException, NotExistingBrancheException);
	Branche& getRoot();


	/* ****************************************************
	 * Distant management
	 * ***************************************************/

	Distant* addDistant(const string& distantName);
	vector<Distant*>& getDistants();
};

#endif /* DATATREE_H_ */
