/*
 * ClientDataTree.h
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#ifndef CLIENTDATATREE_H_
#define CLIENTDATATREE_H_

#include <string>
#include <vector>

using namespace std;

#include "data/DataTree.h"

class ClientDataTree : public DataTree {
	std::string _clientName;
	DistantTreeProxy* _serverTreeProxy;

	Uint32 _updateClientToServer;

	void initDistantBranche(DistantTreeProxy* distant, Branche* branche);
	void initDonneeAndServeurMarqueur(Donnee* donnee);
public:
	ClientDataTree(const std::string& clientName, Interlocutor2* serverInterlocutor);
	virtual ~ClientDataTree();

	const string& getClientName() const;
	Branche* getBrancheByTmpId(const vector<int>& parentBrancheId, int brancheTmpId) throw(NotExistingBrancheException);

	/* *****************************************************
	 * Serveur local actions
	 * ****************************************************/

	// Gestion branches et valeurs avec coordonnées vectorielles
	Branche* createBranche(DistantTreeProxy* distant, const std::vector<int>& parentBrancheId, const std::string& brancheName) override;
	Branche* createPrivateBranche(const std::vector<int>& parentBrancheId, const std::string& brancheName) override;

	Valeur* createValeur(DistantTreeProxy* distant, UPDATE_MODE updateMode, const std::vector<int>& parentBrancheId, const std::string& valeurName, const JktUtils::AnyData valeur) override;


	/* ****************************************************
	 * Distants management
	 * ***************************************************/

	DistantTreeProxy* getDistantServer() const;

	/* ****************************************************
	 * Communication
	 * ***************************************************/

	void diffuseChangementsToServer(void);
	void collecteChangements(vector<Changement*>& changements);
	void receiveChangementsFromServer();
	void sendChangementsToServer(vector<Changement*>& changements);
};

#endif /* CLIENTDATATREE_H_ */
