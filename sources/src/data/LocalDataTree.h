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

#include "util/types/Data.h"

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
	Branche* createBranche(const std::vector<int>& parentBrancheId, const std::string& brancheName);
	Valeur* createValeur(const std::vector<int>& parentBrancheId, const std::string& valeurName, const JktUtils::Data* valeur);
};

#endif /* CLIENTDATATREE_H_ */
