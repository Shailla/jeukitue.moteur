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
	Donnee* addMarqueurFromDistant(Client* client, Donnee* valeur, int donneeClientTmpId);
	void initDistantBranche(Client* client, Branche* branche);

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

	Branche* addBrancheFromDistant(vector<int>& parentBrancheId, const string& brancheName, int brancheTmpId, Client* client);
	Valeur* addValeurIntFromDistant(vector<int>& parentBrancheId, const string& valeurName, int valeurTmpId, int valeur, Client* client);


	/* ****************************************************
	 * Communication
	 * ***************************************************/

	void diffuseChangements(void);
};

#endif /* DATATREE_H_ */
