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
#include "util/types/AnyData.h"
#include "data/TreeConstants.h"
#include "data/Branche.h"
#include "data/PrivateBranche.h"
#include "data/DistantTreeProxy.h"

class DataTree {
	Branche _root;
	TREE_TYPE _treeType;

protected:
	std::set<Donnee*> _donnees;
	virtual void initDistantBranche(DistantTreeProxy* distant, Branche* branche) = 0;

public:
	DataTree(TREE_TYPE treeType);
	virtual ~DataTree();



	/* *****************************************************
	 * Actions
	 * ****************************************************/

	// Gestion branches et valeurs avec coordonnées vectorielles
	virtual Branche* createBranche(DistantTreeProxy* distant, const std::vector<int>& parentBrancheId, const std::string& brancheName) = 0;
	virtual Branche* createPrivateBranche(const std::vector<int>& parentBrancheId, const std::string& brancheName) = 0;

	virtual Valeur* createValeur(DistantTreeProxy* distant, UPDATE_MODE updateMode, const std::vector<int>& parentBrancheId, const std::string& valeurName, const JktUtils::AnyData valeur) = 0;


	/* *********************************
	 * Accessors
	 * ********************************/

	Branche& getRoot();

	Branche* getBrancheFromDistant(DistantTreeProxy* distant, const vector<int>& brancheId) throw(NotExistingBrancheException);
	Valeur* getValeurFromDistant(DistantTreeProxy* distant, const vector<int>& brancheId, int valeurId) throw(NotExistingValeurException, NotExistingBrancheException);

	/**
	 * Retourne la branche idéntifiée par son identifiant.
	 *
	 * @param brancheId coordonnées de la branche dans l'arbre
	 */
	Branche* getBranche(DistantTreeProxy* distant, const vector<int>& brancheId) throw(NotExistingBrancheException);

	/**
	 * Retourne la branche idéntifiée par son chemin.
	 *
	 * @param branchePath chemin d'accès à la branche
	 */
	Branche* getBranche(DistantTreeProxy* distant, const vector<string>& branchePath) throw(NotExistingBrancheException);

	Valeur* getValeur(DistantTreeProxy* distant, const vector<int>& brancheId, int valeurId) throw(NotExistingValeurException, NotExistingBrancheException);

	/** Indique si la branche ou valeur est contenue dans l'arbre. */
	bool containsDonnee(Donnee* donnee);
};

#endif /* DATATREE_H_ */
