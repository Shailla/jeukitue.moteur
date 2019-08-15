/*
 * LocalDataTree.h
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#ifndef LOCALDATATREE_H_
#define LOCALDATATREE_H_

#include <string>
#include <vector>

#include "data/DataTree.h"

class LocalDataTree : public DataTree {
public:
	LocalDataTree();
	virtual ~LocalDataTree();

	void initDistantBranche(DistantTreeProxy* distant, Branche* branche);


	/* *****************************************************
	 * Serveur local actions
	 * ****************************************************/

	// Gestion branches et valeurs avec coordonn�es vectorielles
	Branche* createBranche(DistantTreeProxy* distant, const std::vector<int>& parentBrancheId, const std::string& brancheName) override;
	Branche* createPrivateBranche(const std::vector<int>& parentBrancheId, const std::string& brancheName) override;

	Valeur* createValeur(DistantTreeProxy* distant, UPDATE_MODE updateMode, const std::vector<int>& parentBrancheId, const std::string& valeurName, const jkt::AnyData valeur) override;

	Branche* getBrancheFromDistant(DistantTreeProxy* distant, const std::vector<int>& brancheId) noexcept(false) override;

	Valeur* getValeurFromDistant(DistantTreeProxy* distant, const std::vector<int>& brancheId, int valeurId) noexcept(false) override;
};

#endif /* CLIENTDATATREE_H_ */
