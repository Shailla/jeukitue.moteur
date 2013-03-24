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

#include "data/DataTree.h"

class ServeurDataTree : public DataTree {
	std::vector<Distant*> _clients;

	Donnee* addMarqueurFromDistant(Distant* distant, Donnee* valeur, int donneeClientTmpId);
	void initDistantBranche(Distant* distant, Branche* branche);

public:
	ServeurDataTree();
	virtual ~ServeurDataTree();


	/* *****************************************************
	 * Serveur local actions
	 * ****************************************************/

	Branche* createBranche(std::vector<int>& parentBrancheId, const std::string& brancheName);
	Valeur* createValeurInt(std::vector<int>& parentBrancheId, const std::string& valeurName, int valeur);


	/* ****************************************************
	 * Distant actions
	 * ***************************************************/

	Branche* addBrancheFromDistant(vector<int>& parentBrancheId, const std::string& brancheName, int brancheTmpId, Distant* client);
	Valeur* addValeurIntFromDistant(vector<int>& parentBrancheId, const std::string& valeurName, int valeurTmpId, int valeur, Distant* client);


	/* ****************************************************
	 * Distants management
	 * ***************************************************/

	Distant* addDistant(const std::string& distantName);
	std::vector<Distant*>& getDistants();


	/* ****************************************************
	 * Communication
	 * ***************************************************/

	void diffuseChangementsToClients(void);
	void receiveChangementsFromClient();
};

#endif /* DATATREE_H_ */
