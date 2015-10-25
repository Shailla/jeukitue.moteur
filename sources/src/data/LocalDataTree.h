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

using namespace std;

#include "data/DataTree.h"

class LocalDataTree : public DataTree {
public:
	LocalDataTree();
	virtual ~LocalDataTree();

	void initDistantBranche(DistantTreeProxy* distant, Branche* branche);


	/* *****************************************************
	 * Serveur local actions
	 * ****************************************************/

	// Gestion branches et valeurs avec coordonnées vectorielles
	Branche* createBranche(DistantTreeProxy* distant, const std::vector<int>& parentBrancheId, const std::string& brancheName) override;
	Branche* createPrivateBranche(const std::vector<int>& parentBrancheId, const std::string& brancheName) override;

	Valeur* createValeur(DistantTreeProxy* distant, UPDATE_MODE updateMode, const std::vector<int>& parentBrancheId, const std::string& valeurName, const JktUtils::AnyData valeur) override;

	Branche* getBrancheFromDistant(DistantTreeProxy* distant, const vector<int>& brancheId) throw(NotExistingBrancheException) override;

	Valeur* getValeurFromDistant(DistantTreeProxy* distant, const vector<int>& brancheId, int valeurId) throw(NotExistingValeurException, NotExistingBrancheException) override;
};

#endif /* CLIENTDATATREE_H_ */
