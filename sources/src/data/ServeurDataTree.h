/*
 * ServeurDataTree.h
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

	Branche* createBranche(const std::vector<int>& parentBrancheId, const std::string& brancheName, int revision);
	Valeur* createValeurInt(const std::vector<int>& parentBrancheId, const std::string& valeurName, int revision, int valeur);
	Valeur* updateValeurInt(const std::vector<int>& brancheId, int valeurId, int valeur);


	/* ****************************************************
	 * Distant actions
	 * ***************************************************/

	Branche* getBrancheByDistantTmpId(Distant* distant, const vector<int>& parentBrancheId, int brancheTmpId) throw(NotExistingBrancheException);
	Valeur* getValeurByDistantTmpId(Distant* distant, const vector<int>& brancheId, int valeurTmpId) throw(NotExistingBrancheException, NotExistingValeurException);

	Branche* addBrancheFromDistant(const vector<int>& parentBrancheId, const std::string& brancheName, int brancheTmpId, int revision, Distant* distant);
	Valeur* addValeurIntFromDistant(const vector<int>& parentBrancheId, const std::string& valeurName, int valeurTmpId, int revision, int valeur, Distant* distant);


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

#endif /* SERVEURDATATREE_H_ */
