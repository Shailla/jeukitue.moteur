/*
 * DataTree.h
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#ifndef CLIENTDATATREE_H_
#define CLIENTDATATREE_H_

#include <string>
#include <map>
#include <vector>

using namespace std;

#include "data/DataTree.h"

class ClientDataTree : public DataTree {
	Distant* _serveur;

	void initDistantBranche(Distant* distant, Branche* branche);

public:
	ClientDataTree(Distant* server);
	virtual ~ClientDataTree();


	/* ****************************************************
	 * Distants management
	 * ***************************************************/

	Distant* getDistantServer() const;

	/* ****************************************************
	 * Communication
	 * ***************************************************/

	void receiveChangementsFromServer(const std::string& data, std::vector<Changement*>& confirmations);
	void sendChangementsToServer(vector<Changement*>& changements);
};

#endif /* DATATREE_H_ */
