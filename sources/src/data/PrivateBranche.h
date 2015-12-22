/*
 * Branche.h
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#ifndef PRIVATE_BRANCHE_H_
#define PRIVATE_BRANCHE_H_

#include <sstream>
#include <string>
#include <map>
#include <vector>

#include "util/GenRef.h"
#include "data/exception/NotExistingBrancheException.h"
#include "data/exception/NotExistingValeurException.h"
#include "data/exception/AlreadyExistingBrancheException.h"
#include "data/Donnee.h"
#include "data/Branche.h"
#include "data/Valeur.h"
#include "data/DistantTreeProxy.h"

class PrivateBranche : public Branche {

	class DistantPrivateBranche {
	public:
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
	};

	std::map<DistantTreeProxy*, DistantPrivateBranche> _distants;

public:
	PrivateBranche(AbstractBranche* parent, int brancheId, const std::string& brancheName, DONNEE_TYPE brancheType, int revision, int tmpId);
	virtual ~PrivateBranche();


	/* ************************************************************************ */
	// Fonctions membres
	/* ************************************************************************ */

	DistantPrivateBranche* getDistant(DistantTreeProxy* distant);
	map<DistantTreeProxy*, DistantPrivateBranche>& getDistants();
	DistantPrivateBranche* addDistant(DistantTreeProxy* distant);

	Branche* getSubBrancheByIdOrDistantTmpId(DistantTreeProxy* distant, int brancheId) throw(NotExistingBrancheException);
	Branche* getSubBrancheByDistantTmpId(DistantTreeProxy* distant, int brancheId) throw(NotExistingBrancheException);

	Valeur* getValeurByDistantTmpId(DistantTreeProxy* distant, int valeurTmpId) throw(NotExistingValeurException);

	Branche* getSubBrancheByName(DistantTreeProxy* distant, const string& brancheName) override;
	Branche* getSubBrancheByIdOrTmpId(DistantTreeProxy* distant, int brancheId) override;
	std::vector<Branche*>* getSubBranches(DistantTreeProxy* distant) override;

	std::vector<Valeur*>& getValeurs(DistantTreeProxy* distant);
	Valeur* getValeurByIdOrTmpId(DistantTreeProxy* distant, int valeurId) override;

	/** Attribue son identifiant d�finitf � une branche temporaire */
	Branche* acceptTmpSubBranche(DistantTreeProxy* distant, int brancheTmpId, int brancheId, int brancheRevision) throw(NotExistingBrancheException) override;

	Valeur* acceptTmpValeur(DistantTreeProxy* distant, int valeurTmpId, int valeurId, int valeurRevision) throw(NotExistingValeurException) override;

	/** Cr�e une nouvelle branche et lui attribue un identifiant temporaire */
	Branche* createSubBrancheForClient(DistantTreeProxy* distant, const std::string& brancheName, int revision);

	/** Cr�e une nouvelle branche et lui attribue un identifiant */
	Branche* createSubBrancheForServer(DistantTreeProxy* distant, const std::string& brancheName, DONNEE_TYPE type, int revision) throw(AlreadyExistingBrancheException);

	/** Ajoute une branche qui a d�j� un identifiant car elle a par exemple �t� cr��e sur le serveur puis diffus�e */
	Branche* addSubBranche(DistantTreeProxy* distant, int brancheId, DONNEE_TYPE type, const std::string& brancheName, int brancheRevision) override;

	/** Cr�e une nouvelle valeur enti�re et lui attribue un identifiant temporaire */
	Valeur* createValeurForClient(DistantTreeProxy* distant, UPDATE_MODE updateMode, const std::string& valeurName, int revision, const JktUtils::AnyData& valeur);

	/** Cr�e une nouvelle valeur enti�re et lui attribue un identifiant */
	Valeur* createValeurForServeur(DistantTreeProxy* distant, UPDATE_MODE updateMode, const std::string& valeurName, int revision, const JktUtils::AnyData& valeur) override;

	/** Ajoute une valeur enti�re qui a d�j� un identifiant car elle a par exemple �t� cr��e sur le serveur puis diffus�e */
	Valeur* addValeur(DistantTreeProxy* distant, UPDATE_MODE updateMode, int valeurId, const std::string& valeurName, int valeurRevision, const JktUtils::AnyData& valeur) override;

	/**
	 * Affiche le sous-arbre et ses donn�es et caract�ristiques partag�es avec les autres arbres.
	 * Si details=false seules les informations partag�es sont affich�es, donc 2 arbres synchronis�s
	 * et stabilis�s g�n�reront exactement le m�me affichage avec cette m�thode.
	 * Si tmpId=true alors les identifiants temporaires sont affich�s.
	 * Si publicOnly=true alors les donn�es priv�es ne sont affich�es.
	 */
	void print(std::ostringstream& out, DistantTreeProxy* distant, bool tmpId, bool publicOnly, int indentation) override;
};

#endif /* PRIVATE_BRANCHE_H_ */
