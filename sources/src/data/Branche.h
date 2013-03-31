/*
 * Branche.h
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#ifndef BRANCHE_H_
#define BRANCHE_H_

#include <string>
#include <map>

#include "util/GenRef.h"
#include "util/types/Data.h"
#include "data/exception/NotExistingBrancheException.h"
#include "data/exception/NotExistingValeurException.h"
#include "data/Donnee.h"
#include "data/Valeur.h"
#include "data/DistantTreeProxy.h"

class Branche : public Donnee {
	Branche* _parent;
	int _brancheId;
	int _brancheTmpId;
	std::string _brancheName;

	std::map<int, Branche*> _subBranches;
	std::map<int, Valeur*> _valeurs;

	JktUtils::CGenRef _brancheTmpRefGenerator;
	JktUtils::CGenRef _brancheRefGenerator;
	JktUtils::CGenRef _valeurRefGenerator;
	JktUtils::CGenRef _valeurTmpRefGenerator;
public:
	Branche(Branche* parent, int brancheId, const std::string& brancheName, int revision, int tmpId);
	virtual ~Branche();

	Branche* getSubBrancheByDistantTmpId(DistantTreeProxy* distant, int brancheTmpId) throw(NotExistingBrancheException);
	Branche* getSubBrancheByTmpId(int brancheTmpId) const;
	Branche* getSubBranche(int brancheId) const;
	std::map<int, Branche*>& getSubBranches();
	void setBrancheId(int brancheId);

	std::map<int, Valeur*>& getValeurs();
	Valeur* getValeur(int valeurId);
	Valeur* getValeurByDistantTmpId(DistantTreeProxy* distant, int valeurTmpId) throw(NotExistingValeurException);
	Valeur* getValeurByTmpId(int valeurTmpId) const;

	std::string getBrancheName() const;
	int getBrancheId() const;
	int getBrancheTmpId() const;
	std::vector<int> getParentBrancheId(void) const;
	void getBrancheFullId(std::vector<int>& id) const;
	std::vector<int> getBrancheFullId() const;

	/** Attribue son identifiant définitf à une branche temporaire */
	Branche* acceptTmpSubBranche(int brancheTmpId, int brancheId, int brancheRevision) throw(NotExistingBrancheException);

	Valeur* acceptTmpValeur(int valeurTmpId, int valeurId, int valeurRevision) throw(NotExistingValeurException);

	/** Crée une nouvelle branche et lui attribue un identifiant temporaire */
	Branche* createSubBrancheForClient(const std::string& brancheName, int revision);

	/** Crée une nouvelle branche et lui attribue un identifiant */
	Branche* createSubBrancheForServer(const std::string& brancheName, int revision);

	/** Ajoute une branche qui a déjà un identifiant car elle a par exemple été créée sur le serveur puis diffusée */
	Branche* addSubBranche(int brancheId, const std::string& brancheName, int brancheRevision);

	/** Crée une nouvelle valeur entière et lui attribue un identifiant temporaire */
	Valeur* createValeurForClient(const std::string& valeurName, int revision, const JktUtils::Data* valeur);

	/** Crée une nouvelle valeur entière et lui attribue un identifiant */
	Valeur* createValeurForServeur(const std::string& valeurName, int revision, const JktUtils::Data* valeur);

	/** Ajoute une valeur entière qui a déjà un identifiant car elle a par exemple été créée sur le serveur puis diffusée */
	const Valeur* addValeurInt(int valeurId, const std::string& valeurName, int valeurRevision, const JktUtils::Data* valeur);
};

#endif /* BRANCHE_H_ */
