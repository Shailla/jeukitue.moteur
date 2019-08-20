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

class PrivateBranche;
class DistantTreeProxy;
class Interlocutor2;

class ServeurDataTree : public DataTree {
	std::vector<DistantTreeProxy*> _clients;

	Donnee* initDonneeAndMarqueurFromDistant(DistantTreeProxy* distant, Donnee* valeur, int donneeClientTmpId);
	void addDistantToPrivateBranches(DistantTreeProxy* distant, Branche* branche);
	void initDistantBranche(DistantTreeProxy* distant, Branche* branche);

	Branche* getBrancheFromDistant(DistantTreeProxy* distant, const std::vector<int>& brancheId) noexcept(false);
	Valeur* getValeurFromDistant(DistantTreeProxy* distant, const std::vector<int>& brancheId, int valeurId) noexcept(false);

	/* ****************************************************
	 * Distant actions
	 * ***************************************************/

	Branche* getBrancheByDistantTmpId(DistantTreeProxy* distant, const std::vector<int>& parentBrancheId, int brancheTmpId) noexcept(false);
	Valeur* getValeurByDistantTmpId(DistantTreeProxy* distant, const std::vector<int>& brancheId, int valeurTmpId) noexcept(false);

	Branche* addBrancheFromDistant(const std::vector<int>& parentBrancheId, const std::string& brancheName, int brancheTmpId, int revision, DistantTreeProxy* distant);
	PrivateBranche* addPrivateBrancheFromDistant(const std::vector<int>& parentBrancheId, const std::string& brancheName, int brancheTmpId, int revision, DistantTreeProxy* distant);
	Valeur* addValeurFromDistant(const std::vector<int>& parentBrancheId, UPDATE_MODE updateMode, const std::string& valeurName, int valeurTmpId, int revision, const jkt::AnyData valeur, DistantTreeProxy* distant);

public:
	ServeurDataTree();
	virtual ~ServeurDataTree();


	/* *****************************************************
	 * Serveur local actions
	 * ****************************************************/

	// Gestion branches et valeurs par référence
	void updateValeur(Valeur* valeur, const jkt::AnyData value);

	// Gestion branches et valeurs avec coordonnées vectorielles
	Branche* createBranche(DistantTreeProxy* distant, const std::vector<int>& parentBrancheId, const std::string& brancheName) override;
	Branche* createPrivateBranche(const std::vector<int>& parentBrancheId, const std::string& brancheName);

	Valeur* createValeur(DistantTreeProxy* distant, UPDATE_MODE updateMode, const std::vector<int>& parentBrancheId, const std::string& valeurName, const jkt::AnyData valeur) override;
	Valeur* updateValeur(const std::vector<int>& brancheId, int valeurId, const jkt::AnyData value);


	/* ****************************************************
	 * Distants management
	 * ***************************************************/

	DistantTreeProxy* addDistant(Interlocutor2* interlocutor);
	std::vector<DistantTreeProxy*>& getDistants();

public:

	/* ****************************************************
	 * Communication
	 * ***************************************************/

	void diffuseChangementsToClients(void);
	void collecteChangements(std::vector<Changement*>& changements, DistantTreeProxy* distant);
	void receiveChangementsFromClients();
};

#endif /* SERVEURDATATREE_H_ */
