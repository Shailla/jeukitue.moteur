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

	std::stack<Branche*> _br;
	std::stack<int> _pos;
	std::stack<int> _size;

	BrancheIterator(Branche* origin, DistantTreeProxy* distant);

	void push(Branche* br, int pos, int size);
	void pop();

public:
	bool operator++();
	Branche* operator*() const;
};

class Branche : public AbstractBranche {
	std::vector<Branche*> _subBranches;
	std::map<int, Branche*> _subBranchesById;
	std::map<int, Branche*> _subBranchesByTmpId;
	std::map<std::string, Branche*> _subBranchesByName;

	std::vector<Valeur*> _valeurs;
	std::map<int, Valeur*> _valeursById;
	std::map<int, Valeur*> _valeursByTmpId;
	std::map<std::string, Valeur*> _valeursByName;

	jkt::CGenRef _brancheTmpRefGenerator;
	jkt::CGenRef _brancheRefGenerator;
	jkt::CGenRef _valeurRefGenerator;
	jkt::CGenRef _valeurTmpRefGenerator;

public:
	/** Create a root branche */
	Branche(DataTree* dataTree, int brancheId, DONNEE_TYPE brancheType, int revision, int tmpId);
	/** Create a branche */
	Branche(AbstractBranche* parent, int brancheId, const std::string& brancheName, DONNEE_TYPE brancheType, int revision, int tmpId);
	virtual ~Branche();

	BrancheIterator begin(DistantTreeProxy* distant);

	/* ****************************************************** */
	// Fonctions compatibles avec les branches privées
	/* ****************************************************** */

	Branche* getSubBrancheByIdOrDistantTmpId(DistantTreeProxy* distant, int brancheId) throw(NotExistingBrancheException);
	Branche* getSubBrancheByDistantTmpId(DistantTreeProxy* distant, int brancheId) throw(NotExistingBrancheException);
	Valeur* getValeurByDistantTmpId(DistantTreeProxy* distant, int valeurTmpId) throw(NotExistingValeurException);

	/** Crée une nouvelle branche et lui attribue un identifiant temporaire */
	Branche* createSubBrancheForClient(const std::string& brancheName, int revision);

	/** Crée une nouvelle valeur entière et lui attribue un identifiant temporaire */
	Valeur* createValeurForClient(UPDATE_MODE updateMode, const std::string& valeurName, int revision, const jkt::AnyData& valeur);
	virtual Valeur* getValeurByName(DistantTreeProxy* distant, const std::string& valeurName);

	/* ****************************************************** */
	// Fonctions non-compatibles avec les branches privées
	/* ****************************************************** */

	virtual Branche* getSubBrancheByName(DistantTreeProxy* distant, const std::string& brancheName);
	virtual Branche* getSubBrancheByIdOrTmpId(DistantTreeProxy* distant, int brancheId);
	virtual std::vector<Branche*>* getSubBranches(DistantTreeProxy* distant);

	virtual std::vector<Valeur*>& getValeurs(DistantTreeProxy* distant);
	virtual Valeur* getValeurByIdOrTmpId(DistantTreeProxy* distant, int valeurId);

	/** Attribue son identifiant définitf à une branche temporaire */
	virtual Branche* acceptTmpSubBranche(DistantTreeProxy* distant, int brancheTmpId, int brancheId, int brancheRevision) throw(NotExistingBrancheException);

	virtual Valeur* acceptTmpValeur(DistantTreeProxy* distant, int valeurTmpId, int valeurId, int valeurRevision) throw(NotExistingValeurException);

	/** Crée une nouvelle branche et lui attribue un identifiant */
	virtual Branche* createSubBrancheForServer(DistantTreeProxy* distant, const std::string& brancheName, DONNEE_TYPE type, int revision) throw(AlreadyExistingBrancheException);

	/** Ajoute une branche qui a déjà un identifiant car elle a par exemple été créée sur le serveur puis diffusée */
	virtual Branche* addSubBranche(DistantTreeProxy* distant, int brancheId, DONNEE_TYPE type, const std::string& brancheName, int brancheRevision);

	/** Crée une nouvelle valeur entière et lui attribue un identifiant */
	virtual Valeur* createValeurForServeur(DistantTreeProxy* distant, UPDATE_MODE updateMode, const std::string& valeurName, int revision, const jkt::AnyData& valeur);

	/** Ajoute une valeur entière qui a déjà un identifiant car elle a par exemple été créée sur le serveur puis diffusée */
	virtual Valeur* addValeur(DistantTreeProxy* distant, UPDATE_MODE updateMode, int valeurId, const std::string& valeurName, int valeurRevision, const jkt::AnyData& valeur);

	/**
	 * Affiche le sous-arbre et ses données et caractéristiques partagées avec les autres arbres.
	 * Si details=false seules les informations partagées sont affichées, donc 2 arbres synchronisés
	 * et stabilisés généreront exactement le même affichage avec cette méthode.
	 * Si tmpId=true alors les identifiants temporaires sont affichés.
	 * Si publicOnly=true alors les données privées ne sont affichées.
	 */
	virtual void print(std::ostringstream& out, DistantTreeProxy* distant, bool tmpId, bool publicOnly, int indentation) override;

	static bool highestId(const Branche* left, const Branche* right);
};

#endif /* BRANCHE_H_ */
