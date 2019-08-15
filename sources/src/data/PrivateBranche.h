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
public:
	class DistantPrivateBranche {
	public:
		std::vector<Branche*> _subBranches;
		std::map<int, Branche*> _subBranchesById;
		std::map<int, Branche*> _subBranchesByTmpId;
		std::map<std::string, Branche*> _subBranchesByName;

		std::vector<Valeur*> _valeurs;
		std::map<int, Valeur*> _valeursById;
		std::map<int, Valeur*> _valeursByTmpId;

		jkt::CGenRef _brancheTmpRefGenerator;
		jkt::CGenRef _brancheRefGenerator;
		jkt::CGenRef _valeurRefGenerator;
		jkt::CGenRef _valeurTmpRefGenerator;
	};

	std::map<DistantTreeProxy*, DistantPrivateBranche> _distants;

	PrivateBranche(AbstractBranche* parent, int brancheId, const std::string& brancheName, DONNEE_TYPE brancheType, int revision, int tmpId);
	virtual ~PrivateBranche();


	/* ************************************************************************ */
	// Fonctions membres
	/* ************************************************************************ */

	DistantPrivateBranche* getDistant(DistantTreeProxy* distant);
	std::map<DistantTreeProxy*, DistantPrivateBranche>& getDistants();
	DistantPrivateBranche* addDistant(DistantTreeProxy* distant);

	Branche* getSubBrancheByIdOrDistantTmpId(DistantTreeProxy* distant, int brancheId) noexcept(false);
	Branche* getSubBrancheByDistantTmpId(DistantTreeProxy* distant, int brancheId) noexcept(false);

	Valeur* getValeurByDistantTmpId(DistantTreeProxy* distant, int valeurTmpId) noexcept(false);

	Branche* getSubBrancheByName(DistantTreeProxy* distant, const std::string& brancheName) override;
	Branche* getSubBrancheByIdOrTmpId(DistantTreeProxy* distant, int brancheId) override;
	std::vector<Branche*>* getSubBranches(DistantTreeProxy* distant) override;

	std::vector<Valeur*>& getValeurs(DistantTreeProxy* distant);
	Valeur* getValeurByIdOrTmpId(DistantTreeProxy* distant, int valeurId) override;

	/** Attribue son identifiant d�finitf � une branche temporaire */
	Branche* acceptTmpSubBranche(DistantTreeProxy* distant, int brancheTmpId, int brancheId, int brancheRevision) noexcept(false) override;

	Valeur* acceptTmpValeur(DistantTreeProxy* distant, int valeurTmpId, int valeurId, int valeurRevision) noexcept(false) override;

	/** Cr�e une nouvelle branche et lui attribue un identifiant temporaire */
	Branche* createSubBrancheForClient(DistantTreeProxy* distant, const std::string& brancheName, int revision);

	/** Cr�e une nouvelle branche et lui attribue un identifiant */
	Branche* createSubBrancheForServer(DistantTreeProxy* distant, const std::string& brancheName, DONNEE_TYPE type, int revision) noexcept(false);

	/** Ajoute une branche qui a d�j� un identifiant car elle a par exemple �t� cr��e sur le serveur puis diffus�e */
	Branche* addSubBranche(DistantTreeProxy* distant, int brancheId, DONNEE_TYPE type, const std::string& brancheName, int brancheRevision) override;

	/** Cr�e une nouvelle valeur enti�re et lui attribue un identifiant temporaire */
	Valeur* createValeurForClient(DistantTreeProxy* distant, UPDATE_MODE updateMode, const std::string& valeurName, int revision, const jkt::AnyData& valeur);

	/** Cr�e une nouvelle valeur enti�re et lui attribue un identifiant */
	Valeur* createValeurForServeur(DistantTreeProxy* distant, UPDATE_MODE updateMode, const std::string& valeurName, int revision, const jkt::AnyData& valeur) override;

	/** Ajoute une valeur enti�re qui a d�j� un identifiant car elle a par exemple �t� cr��e sur le serveur puis diffus�e */
	Valeur* addValeur(DistantTreeProxy* distant, UPDATE_MODE updateMode, int valeurId, const std::string& valeurName, int valeurRevision, const jkt::AnyData& valeur) override;

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
