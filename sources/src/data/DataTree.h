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
#include "data/exception/NotExistingBrancheException.h"

class DataTree {
	Branche _root;
	vector<Client*> _clients;
public:
	DataTree();
	virtual ~DataTree();

	// Accessors

	/**
	 * Retourne la branche id�ntifi�e.
	 *
	 * @param brancheId coordonn�es de la branche dans l'arbre
	 */
	Branche* getBranche(vector<int> brancheId) throw(NotExistingBrancheException);
	Branche& getRoot();

	Branche* addBranche(vector<int>& parentBrancheId, const string& brancheName);
	Branche* addBrancheForClient(vector<int>& parentBrancheId, const string& brancheName, int brancheTmpId, Client* client);

	Valeur* addValeurInt(vector<int>& parentBrancheId, const string& valeurName, int valeur);
	Valeur* addValeurIntForClient(vector<int>& parentBrancheId, const string& valeurName, int valeurTmpId, int valeur, Client* client);

	void addClient(const string& clientName);
	vector<Client*>& getClients();

	void diffuseChangements();
};

#endif /* DATATREE_H_ */