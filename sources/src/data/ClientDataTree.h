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

#include "util/types/Data.h"

#include "data/DataTree.h"

class ClientDataTree : public DataTree {
	std::string _clientName;
	DistantTreeProxy* _serverTreeProxy;

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
	Branche* createBranche(const std::vector<int>& parentBrancheId, const std::string& brancheName);
	Valeur* createValeur(const std::vector<int>& parentBrancheId, const std::string& valeurName, const JktUtils::Data* valeur);


	/* ****************************************************
	 * Distants management
	 * ***************************************************/

	DistantTreeProxy* getDistantServer() const;

	/* ****************************************************
	 * Communication
	 * ***************************************************/

	void diffuseChangementsToServer(void);
	void receiveChangementsFromServer();
	void sendChangementsToServer(vector<Changement*>& changements);
};

#endif /* CLIENTDATATREE_H_ */
