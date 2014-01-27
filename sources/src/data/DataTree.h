/*
 * DataTree.h
 *
 *  Created on: 15 mars 2013
 *      Author: Erwin
 */

#ifndef DATATREE_H_
#define DATATREE_H_

#include <set>

#include "data/exception/NotExistingBrancheException.h"
#include "data/exception/NotExistingValeurException.h"
#include "util/types/Data.h"
#include "data/Branche.h"
#include "data/DistantTreeProxy.h"

class DataTree {
	Branche _root;

protected:
	std::set<Donnee*> _donnees;
	virtual void initDistantBranche(DistantTreeProxy* distant, Branche* branche) = 0;

public:
	DataTree();
	virtual ~DataTree();


	/* *****************************************************
	 * Actions
	 * ****************************************************/

	// Gestion branches et valeurs avec coordonnées vectorielles
	virtual Branche* createBranche(const std::vector<int>& parentBrancheId, const std::string& brancheName) = 0;
	virtual Valeur* createValeur(const std::vector<int>& parentBrancheId, const std::string& valeurName, const JktUtils::Data* valeur) = 0;


	/* *********************************
	 * Accessors
	 * ********************************/

	Branche& getRoot();

	Branche* getBrancheFromDistant(DistantTreeProxy* distant, const vector<int>& brancheId) throw(NotExistingBrancheException);
	Valeur* getValeurFromDistant(DistantTreeProxy* distant, const vector<int>& brancheId, int valeurId) throw(NotExistingValeurException, NotExistingBrancheException);

	/**
	 * Retourne la branche idéntifiée.
	 *
	 * @param brancheId coordonnées de la branche dans l'arbre
	 */
	Branche* getBranche(const vector<int>& brancheId) throw(NotExistingBrancheException);

	Valeur* getValeur(const vector<int>& brancheId, int valeurId) throw(NotExistingValeurException, NotExistingBrancheException);

	/** Indique si la branche ou valeur est contenue dans l'arbre. */
	bool containsDonnee(Donnee* donnee);
};

#endif /* DATATREE_H_ */
