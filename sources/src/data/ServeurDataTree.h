/*
 * DataTree.h
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#ifndef SERVEURDATATREE_H_
#define SERVEURDATATREE_H_

#include <string>
#include <map>
#include <vector>

using namespace std;

#include "data/DataTree.h"

class ServeurDataTree : public DataTree {
	Donnee* addMarqueurFromDistant(Distant* distant, Donnee* valeur, int donneeClientTmpId);
	void initDistantBranche(Distant* distant, Branche* branche);

public:
	ServeurDataTree();
	virtual ~ServeurDataTree();


	/* *****************************************************
	 * Serveur local actions
	 * ****************************************************/

	Branche* addBranche(vector<int>& parentBrancheId, const string& brancheName);
	Valeur* addValeurInt(vector<int>& parentBrancheId, const string& valeurName, int valeur);


	/* ****************************************************
	 * Distant actions
	 * ***************************************************/

	Branche* addBrancheFromDistant(vector<int>& parentBrancheId, const string& brancheName, int brancheTmpId, Distant* client);
	Valeur* addValeurIntFromDistant(vector<int>& parentBrancheId, const string& valeurName, int valeurTmpId, int valeur, Distant* client);


	/* ****************************************************
	 * Communication
	 * ***************************************************/

	void diffuseChangements(void);
};

#endif /* DATATREE_H_ */
