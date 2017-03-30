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

#include "data/DataTree.h"

class ClientDataTree : public DataTree {
	std::string _clientName;
	DistantTreeProxy* _serverTreeProxy;

	Uint32 _updateClientToServer;

	void initDistantBranche(DistantTreeProxy* ignored, Branche* branche);
	MarqueurDistant* initDonneeAndServeurMarqueur(Donnee* donnee);
public:
	ClientDataTree(const std::string& clientName, Interlocutor2* serverInterlocutor);
	virtual ~ClientDataTree();

	const std::string& getClientName() const;
	Branche* getBrancheByTmpId(const std::vector<int>& parentBrancheId, int brancheTmpId) throw(NotExistingBrancheException);

	Branche* getBrancheFromDistant(DistantTreeProxy* ignored, const std::vector<int>& brancheId) throw(NotExistingBrancheException) override;
	Valeur* getValeurFromDistant(DistantTreeProxy* ignored, const std::vector<int>& brancheId, int valeurId) throw(NotExistingValeurException, NotExistingBrancheException) override;

	/* *****************************************************
	 * Serveur local actions
	 * ****************************************************/

	// Gestion branches et valeurs avec coordonnées vectorielles
	Branche* createBranche(DistantTreeProxy* ignored, const std::vector<int>& parentBrancheId, const std::string& brancheName) override;
	Branche* createPrivateBranche(const std::vector<int>& parentBrancheId, const std::string& brancheName) override;

	Valeur* createValeur(DistantTreeProxy* ignored, UPDATE_MODE updateMode, const std::vector<int>& parentBrancheId, const std::string& valeurName, const jkt::AnyData valeur) override;


	/* ****************************************************
	 * Distants management
	 * ***************************************************/

	DistantTreeProxy* getDistantServer() const;

	/* ****************************************************
	 * Communication
	 * ***************************************************/

	void diffuseChangementsToServer(void);
	void collecteChangements(std::vector<Changement*>& changements);
	void receiveChangementsFromServer();
	void sendChangementsToServer(std::vector<Changement*>& changements);
};

#endif /* CLIENTDATATREE_H_ */
