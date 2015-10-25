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
	std::vector<DistantTreeProxy*> _clients;

	Donnee* initDonneeAndMarqueurFromDistant(DistantTreeProxy* distant, Donnee* valeur, int donneeClientTmpId);
	void initDistantBranche(DistantTreeProxy* distant, Branche* branche);

	Branche* getBrancheFromDistant(DistantTreeProxy* distant, const vector<int>& brancheId) throw(NotExistingBrancheException);
	Valeur* getValeurFromDistant(DistantTreeProxy* distant, const vector<int>& brancheId, int valeurId) throw(NotExistingValeurException, NotExistingBrancheException);

	/* ****************************************************
	 * Distant actions
	 * ***************************************************/

	Branche* getBrancheByDistantTmpId(DistantTreeProxy* distant, const vector<int>& parentBrancheId, int brancheTmpId) throw(NotExistingBrancheException);
	Valeur* getValeurByDistantTmpId(DistantTreeProxy* distant, const vector<int>& brancheId, int valeurTmpId) throw(NotExistingBrancheException, NotExistingValeurException);

	Branche* addBrancheFromDistant(const vector<int>& parentBrancheId, const std::string& brancheName, int brancheTmpId, int revision, DistantTreeProxy* distant);
	PrivateBranche* addPrivateBrancheFromDistant(const vector<int>& parentBrancheId, const std::string& brancheName, int brancheTmpId, int revision, DistantTreeProxy* distant);
	Valeur* addValeurFromDistant(const vector<int>& parentBrancheId, UPDATE_MODE updateMode, const std::string& valeurName, int valeurTmpId, int revision, const JktUtils::AnyData valeur, DistantTreeProxy* distant);

public:
	ServeurDataTree();
	virtual ~ServeurDataTree();


	/* *****************************************************
	 * Serveur local actions
	 * ****************************************************/

	// Gestion branches et valeurs par référence
	void updateValeur(Valeur* valeur, const JktUtils::AnyData value);

	// Gestion branches et valeurs avec coordonnées vectorielles
	Branche* createBranche(DistantTreeProxy* distant, const std::vector<int>& parentBrancheId, const std::string& brancheName) override;
	Branche* createPrivateBranche(const std::vector<int>& parentBrancheId, const std::string& brancheName);

	Valeur* createValeur(DistantTreeProxy* distant, UPDATE_MODE updateMode, const std::vector<int>& parentBrancheId, const std::string& valeurName, const JktUtils::AnyData valeur) override;
	Valeur* updateValeur(const std::vector<int>& brancheId, int valeurId, const JktUtils::AnyData value);


	/* ****************************************************
	 * Distants management
	 * ***************************************************/

	DistantTreeProxy* addDistant(Interlocutor2* interlocutor);
	const std::vector<DistantTreeProxy*>& getDistants();
private:
	void initVid(DistantTreeProxy* distant);
public:

	/* ****************************************************
	 * Communication
	 * ***************************************************/

	void diffuseChangementsToClients(void);
	void collecteChangements(vector<Changement*>& changements, DistantTreeProxy* distant);
	void receiveChangementsFromClients();
};

#endif /* SERVEURDATATREE_H_ */
