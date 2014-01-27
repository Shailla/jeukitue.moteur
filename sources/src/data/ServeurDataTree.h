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

#include "util/types/Data.h"

#include "data/DataTree.h"

class ServeurDataTree : public DataTree {
	std::vector<DistantTreeProxy*> _clients;

	Donnee* initDonneeAndMarqueurFromDistant(DistantTreeProxy* distant, Donnee* valeur, int donneeClientTmpId);
	void initDistantBranche(DistantTreeProxy* distant, Branche* branche);

	/* ****************************************************
	 * Distant actions
	 * ***************************************************/

	Branche* getBrancheByDistantTmpId(DistantTreeProxy* distant, const vector<int>& parentBrancheId, int brancheTmpId) throw(NotExistingBrancheException);
	Valeur* getValeurByDistantTmpId(DistantTreeProxy* distant, const vector<int>& brancheId, int valeurTmpId) throw(NotExistingBrancheException, NotExistingValeurException);

	Branche* addBrancheFromDistant(const vector<int>& parentBrancheId, const std::string& brancheName, int brancheTmpId, int revision, DistantTreeProxy* distant);
	Valeur* addValeurFromDistant(const vector<int>& parentBrancheId, const std::string& valeurName, int valeurTmpId, int revision, const JktUtils::Data* valeur, DistantTreeProxy* distant);

public:
	ServeurDataTree();
	virtual ~ServeurDataTree();


	/* *****************************************************
	 * Serveur local actions
	 * ****************************************************/

	// Gestion branches et valeurs par référence
	void updateValeur(Valeur* valeur, const JktUtils::Data* value);

	// Gestion branches et valeurs avec coordonnées vectorielles
	Branche* createBranche(const std::vector<int>& parentBrancheId, const std::string& brancheName);
	Valeur* createValeur(const std::vector<int>& parentBrancheId, const std::string& valeurName, const JktUtils::Data* valeur);
	Valeur* updateValeur(const std::vector<int>& brancheId, int valeurId, const JktUtils::Data* value);


	/* ****************************************************
	 * Distants management
	 * ***************************************************/

	void addDistant(Interlocutor2* interlocutor);
	const std::vector<DistantTreeProxy*>& getDistants();


	/* ****************************************************
	 * Communication
	 * ***************************************************/

	void diffuseChangementsToClients(void);
	void receiveChangementsFromClients();
};

#endif /* SERVEURDATATREE_H_ */
