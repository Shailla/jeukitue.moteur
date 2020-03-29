/*
 * DataTree.h
 *
 *  Created on: 15 mars 2013
 *      Author: Erwin
 */

#ifndef DATATREE_H_
#define DATATREE_H_

#include <set>

#include "util/types/AnyData.h"
#include "data/Branche.h"
#include "data/TreeConstants.h"

class DistantTreeProxy;

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

	virtual Valeur* createValeur(DistantTreeProxy* distant, UPDATE_MODE updateMode, const std::vector<int>& parentBrancheId, const std::string& valeurName, const jkt::AnyData valeur) = 0;


	/* *********************************
	 * Accessors
	 * ********************************/

	Branche& getRoot();

	virtual Branche* getBrancheFromDistant(DistantTreeProxy* distant, const std::vector<int>& brancheId) noexcept(false) = 0;
	virtual Valeur* getValeurFromDistant(DistantTreeProxy* distant, const std::vector<int>& brancheId, int valeurId) noexcept(false) = 0;

	/**
	 * Retourne la branche idéntifiée par son identifiant.
	 *
	 * @param brancheId coordonnées de la branche dans l'arbre
	 */
	Branche* getBranche(DistantTreeProxy* distant, const std::vector<int>& brancheId) noexcept(false);

	/**
	 * Retourne la branche idéntifiée par son chemin.
	 *
	 * @param branchePath chemin d'accès à la branche
	 */
	Branche* getBranche(DistantTreeProxy* distant, const std::vector<std::string>& branchePath) noexcept(false);

	Valeur* getValeur(DistantTreeProxy* distant, const std::vector<int>& brancheId, int valeurId) noexcept(false);

	/** Indique si la branche ou valeur est contenue dans l'arbre. */
	bool containsDonnee(Donnee* donnee);
};

#endif /* DATATREE_H_ */
