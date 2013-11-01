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
#include "util/types/Data.h"
#include "data/Branche.h"
#include "data/DistantTreeProxy.h"

class DataTree {
	Branche _root;

protected:
	virtual void initDistantBranche(DistantTreeProxy* distant, Branche* branche) = 0;

public:
	DataTree();
	virtual ~DataTree();


	/* *****************************************************
	 * Serveur local actions
	 * ****************************************************/

	virtual Branche* createBranche(const std::vector<int>& parentBrancheId, const std::string& brancheName) = 0;

	virtual Valeur* createValeur(const std::vector<int>& brancheId, const std::string& valeurName, const JktUtils::Data* valeur) = 0;
	virtual Valeur* updateValeur(const std::vector<int>& brancheId, int valeurId, const JktUtils::Data* valeur) = 0;


	/* *********************************
	 * Accessors
	 * ********************************/

	Branche& getRoot();

	/**
	 * Retourne la branche idéntifiée.
	 *
	 * @param brancheId coordonnées de la branche dans l'arbre
	 */
	Branche* getBranche(const vector<int>& brancheId) throw(NotExistingBrancheException);

	Valeur* getValeur(const vector<int>& brancheId, int valeurId) throw(NotExistingValeurException, NotExistingBrancheException);
};

#endif /* DATATREE_H_ */
