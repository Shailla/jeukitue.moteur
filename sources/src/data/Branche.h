/*
 * Branche.h
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#ifndef BRANCHE_H_
#define BRANCHE_H_

#include <sstream>
#include <string>
#include <map>
#include <vector>

#include "util/GenRef.h"
#include "data/exception/NotExistingBrancheException.h"
#include "data/exception/NotExistingValeurException.h"
#include "data/exception/AlreadyExistingBrancheException.h"
#include "data/Donnee.h"
#include "data/AbstractBranche.h"
#include "data/Valeur.h"
#include "data/DistantTreeProxy.h"

class Branche : public AbstractBranche {
	std::vector<Branche*> _subBranches;
	std::map<int, Branche*> _subBranchesById;
	std::map<int, Branche*> _subBranchesByTmpId;
	std::map<string, Branche*> _subBranchesByName;

	std::vector<Valeur*> _valeurs;
	std::map<int, Valeur*> _valeursById;
	std::map<int, Valeur*> _valeursByTmpId;

	JktUtils::CGenRef _brancheTmpRefGenerator;
	JktUtils::CGenRef _brancheRefGenerator;
	JktUtils::CGenRef _valeurRefGenerator;
	JktUtils::CGenRef _valeurTmpRefGenerator;

public:
	Branche(AbstractBranche* parent, int brancheId, const std::string& brancheName, DONNEE_TYPE brancheType, int revision, int tmpId);
	virtual ~Branche();

	Branche* getSubBrancheByIdOrDistantTmpId(DistantTreeProxy* distant, int brancheId) throw(NotExistingBrancheException);
	Branche* getSubBrancheByDistantTmpId(DistantTreeProxy* distant, int brancheId) throw(NotExistingBrancheException);

	Valeur* getValeurByDistantTmpId(DistantTreeProxy* distant, int valeurTmpId) throw(NotExistingValeurException);

	Branche* getSubBrancheByName(const string& brancheName) const;
	Branche* getSubBrancheByIdOrTmpId(int brancheId) const;
	std::vector<Branche*>& getSubBranches();

	std::vector<Valeur*>& getValeurs();
	Valeur* getValeurByIdOrTmpId(int valeurId) const;

	/** Attribue son identifiant définitf à une branche temporaire */
	Branche* acceptTmpSubBranche(int brancheTmpId, int brancheId, int brancheRevision) throw(NotExistingBrancheException);

	Valeur* acceptTmpValeur(int valeurTmpId, int valeurId, int valeurRevision) throw(NotExistingValeurException);

	/** Crée une nouvelle branche et lui attribue un identifiant temporaire */
	Branche* createSubBrancheForClient(const std::string& brancheName, int revision);

	/** Crée une nouvelle branche et lui attribue un identifiant */
	Branche* createSubBrancheForServer(const std::string& brancheName, DONNEE_TYPE type, int revision) throw(AlreadyExistingBrancheException);

	/** Ajoute une branche qui a déjà un identifiant car elle a par exemple été créée sur le serveur puis diffusée */
	Branche* addSubBranche(int brancheId, const std::string& brancheName, int brancheRevision);

	/** Crée une nouvelle valeur entière et lui attribue un identifiant temporaire */
	Valeur* createValeurForClient(const std::string& valeurName, int revision, const JktUtils::AnyData& valeur);

	/** Crée une nouvelle valeur entière et lui attribue un identifiant */
	Valeur* createValeurForServeur(const std::string& valeurName, int revision, const JktUtils::AnyData& valeur);

	/** Ajoute une valeur entière qui a déjà un identifiant car elle a par exemple été créée sur le serveur puis diffusée */
	const Valeur* addValeur(int valeurId, const std::string& valeurName, int valeurRevision, const JktUtils::AnyData& valeur);

	/**
	 * Affiche le sous-arbre et ses données et caractéristiques partagées avec les autres arbres.
	 * Si details=false seules les informations partagées sont affichées, donc 2 arbres synchronisés
	 * et stabilisés généreront exactement le même affichage avec cette méthode.
	 * Si details=true alors plus de caractérisques sont affichées.
	 */
	void print(std::ostringstream& out, bool details, int indentation);
};

#endif /* BRANCHE_H_ */
