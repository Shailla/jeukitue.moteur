/*
 * Branche.h
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#ifndef BRANCHE_H_
#define BRANCHE_H_

#include <iterator>
#include <map>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

#include "util/GenRef.h"
#include "data/exception/NotExistingBrancheException.h"
#include "data/exception/NotExistingValeurException.h"
#include "data/exception/AlreadyExistingBrancheException.h"
#include "data/Donnee.h"
#include "data/AbstractBranche.h"
#include "data/Valeur.h"

class DistantTreeProxy;

class BrancheIterator {
	friend Branche;

	Branche* _origin;
	DistantTreeProxy* _distant;
	stack<vector<Branche*>::iterator> _posIt;
	stack<vector<Branche*>::iterator> _posEnd;

	BrancheIterator(Branche* origin, DistantTreeProxy* distant);
	void dig(Branche* origin);

	void push(vector<Branche*>::iterator it, vector<Branche*>::iterator end);
	void pop();

public:
	bool operator++();
	Branche* operator*() const;
};

class Branche : public AbstractBranche {
	std::vector<Branche*> _subBranches;
	std::map<int, Branche*> _subBranchesById;
	std::map<int, Branche*> _subBranchesByTmpId;
	std::map<string, Branche*> _subBranchesByName;

	std::vector<Valeur*> _valeurs;
	std::map<int, Valeur*> _valeursById;
	std::map<int, Valeur*> _valeursByTmpId;
	std::map<string, Valeur*> _valeursByName;

	JktUtils::CGenRef _brancheTmpRefGenerator;
	JktUtils::CGenRef _brancheRefGenerator;
	JktUtils::CGenRef _valeurRefGenerator;
	JktUtils::CGenRef _valeurTmpRefGenerator;

public:
	/** Create a root branche */
	Branche(DataTree* dataTree, int brancheId, DONNEE_TYPE brancheType, int revision, int tmpId);
	/** Create a branche */
	Branche(AbstractBranche* parent, int brancheId, const std::string& brancheName, DONNEE_TYPE brancheType, int revision, int tmpId);
	virtual ~Branche();

	BrancheIterator begin(DistantTreeProxy* distant);

	/* ****************************************************** */
	// Fonctions compatibles avec les branches priv�es
	/* ****************************************************** */

	Branche* getSubBrancheByIdOrDistantTmpId(DistantTreeProxy* distant, int brancheId) throw(NotExistingBrancheException);
	Branche* getSubBrancheByDistantTmpId(DistantTreeProxy* distant, int brancheId) throw(NotExistingBrancheException);
	Valeur* getValeurByDistantTmpId(DistantTreeProxy* distant, int valeurTmpId) throw(NotExistingValeurException);

	/** Cr�e une nouvelle branche et lui attribue un identifiant temporaire */
	Branche* createSubBrancheForClient(const std::string& brancheName, int revision);

	/** Cr�e une nouvelle valeur enti�re et lui attribue un identifiant temporaire */
	Valeur* createValeurForClient(UPDATE_MODE updateMode, const std::string& valeurName, int revision, const JktUtils::AnyData& valeur);
	virtual Valeur* getValeurByName(DistantTreeProxy* distant, const string& valeurName);

	/* ****************************************************** */
	// Fonctions non-compatibles avec les branches priv�es
	/* ****************************************************** */

	virtual Branche* getSubBrancheByName(DistantTreeProxy* distant, const string& brancheName);
	virtual Branche* getSubBrancheByIdOrTmpId(DistantTreeProxy* distant, int brancheId);
	virtual std::vector<Branche*>& getSubBranches(DistantTreeProxy* distant);

	virtual std::vector<Valeur*>& getValeurs(DistantTreeProxy* distant);
	virtual Valeur* getValeurByIdOrTmpId(DistantTreeProxy* distant, int valeurId);

	/** Attribue son identifiant d�finitf � une branche temporaire */
	virtual Branche* acceptTmpSubBranche(DistantTreeProxy* distant, int brancheTmpId, int brancheId, int brancheRevision) throw(NotExistingBrancheException);

	virtual Valeur* acceptTmpValeur(DistantTreeProxy* distant, int valeurTmpId, int valeurId, int valeurRevision) throw(NotExistingValeurException);

	/** Cr�e une nouvelle branche et lui attribue un identifiant */
	virtual Branche* createSubBrancheForServer(DistantTreeProxy* distant, const std::string& brancheName, DONNEE_TYPE type, int revision) throw(AlreadyExistingBrancheException);

	/** Ajoute une branche qui a d�j� un identifiant car elle a par exemple �t� cr��e sur le serveur puis diffus�e */
	virtual Branche* addSubBranche(DistantTreeProxy* distant, int brancheId, DONNEE_TYPE type, const std::string& brancheName, int brancheRevision);

	/** Cr�e une nouvelle valeur enti�re et lui attribue un identifiant */
	virtual Valeur* createValeurForServeur(DistantTreeProxy* distant, UPDATE_MODE updateMode, const std::string& valeurName, int revision, const JktUtils::AnyData& valeur);

	/** Ajoute une valeur enti�re qui a d�j� un identifiant car elle a par exemple �t� cr��e sur le serveur puis diffus�e */
	virtual const Valeur* addValeur(DistantTreeProxy* distant, UPDATE_MODE updateMode, int valeurId, const std::string& valeurName, int valeurRevision, const JktUtils::AnyData& valeur);

	/**
	 * Affiche le sous-arbre et ses donn�es et caract�ristiques partag�es avec les autres arbres.
	 * Si details=false seules les informations partag�es sont affich�es, donc 2 arbres synchronis�s
	 * et stabilis�s g�n�reront exactement le m�me affichage avec cette m�thode.
	 * Si details=true alors plus de caract�risques sont affich�es.
	 */
	virtual void print(std::ostringstream& out, DistantTreeProxy* distant, bool details, int indentation) override;

	static bool highestId(const Branche* left, const Branche* right);
};

#endif /* BRANCHE_H_ */
