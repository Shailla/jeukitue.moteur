/*
 * DataTree.h
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#ifndef DATATREE_H_
#define DATATREE_H_

#include <string>
#include <map>
#include <vector>

using namespace std;

#include "data/Branche.h"
#include "data/Client.h"

class DataTree {
	Branche _root;
	vector<Client> _clients;
public:
	DataTree();
	virtual ~DataTree();

	// Accessors

	/**
	 * Retourne la branche idéntifiée.
	 *
	 * @param brancheId coordonnées de la branche dans l'arbre
	 */
	Branche* getBranche(vector<int> brancheId);
	Branche& getRoot();

	Branche* addBranche(vector<int> parentBrancheId);
	Branche* addBrancheForClient(vector<int> parentBrancheId, int brancheTmpId, Client* client);
	void diffuseChangements();
};

#endif /* DATATREE_H_ */
