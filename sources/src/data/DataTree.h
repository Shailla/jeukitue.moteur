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
	vector<Client*> _clients;

public:
	DataTree();
	virtual ~DataTree();


	virtual void initDistantBranche(Client* client, Branche* branche) = 0;

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

	Client* addDistant(const string& clientName);
	vector<Client*>& getDistants();
};

#endif /* DATATREE_H_ */
