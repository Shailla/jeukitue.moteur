/*
 * Branche.h
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#ifndef ABSTRACTBRANCHE_H_
#define ABSTRACTBRANCHE_H_

#include <sstream>
#include <string>
#include <map>
#include <vector>

#include "util/GenRef.h"
#include "data/exception/NotExistingBrancheException.h"
#include "data/exception/NotExistingValeurException.h"
#include "data/exception/AlreadyExistingBrancheException.h"
#include "data/Donnee.h"
#include "data/Valeur.h"
#include "data/DistantTreeProxy.h"

class Branche;

class AbstractBranche : public Donnee {
protected:
	AbstractBranche* _parent;
	int _brancheId;
	int _brancheTmpId;
	std::string _brancheName;

	jkt::CGenRef _brancheTmpRefGenerator;
	jkt::CGenRef _brancheRefGenerator;
	jkt::CGenRef _valeurRefGenerator;
	jkt::CGenRef _valeurTmpRefGenerator;

public:
	AbstractBranche(DataTree* dataTree, int brancheId, const std::string& brancheName, DONNEE_TYPE brancheType, int revision, int brancheTmpId);
	AbstractBranche(AbstractBranche* parent, int brancheId, const std::string& brancheName, DONNEE_TYPE brancheType, int revision, int brancheTmpId);
	virtual ~AbstractBranche();

	void setBrancheId(int brancheId);
	int getBrancheId() const;
	int getBrancheTmpId() const;

	std::string getBrancheName() const;

	void getBrancheFullId(std::vector<int>& id) const;
	std::vector<int> getBrancheFullId() const;
	void getBrancheFullIdOrTmpId(std::vector<int>& id) const;
	std::vector<int> getBrancheFullIdOrTmpId() const;

	std::vector<int> getParentBrancheId(void) const;
	std::vector<int> getParentBrancheIdOrTmpId(void) const;

	virtual Branche* getSubBrancheByIdOrDistantTmpId(DistantTreeProxy* distant, int brancheId) noexcept(false) = 0;
	virtual Branche* getSubBrancheByDistantTmpId(DistantTreeProxy* distant, int brancheId) noexcept(false) = 0;

	virtual Valeur* getValeurByDistantTmpId(DistantTreeProxy* distant, int valeurTmpId) noexcept(false) = 0;

	/**
	 * Affiche le sous-arbre et ses donn�es et caract�ristiques partag�es avec les autres arbres.
	 * 2 arbres synchronis�s et stabilis�s g�n�reront exactement le m�me affichage avec cette m�thode.
	 *
	 * @param distant distant dont l'abre doit �tre affich�, 0 pour afficher l'abre complet avec tous les distants
	 *
	 */
	std::string print(DistantTreeProxy* distant, bool details, bool publicOnly, int indentation);

	/**
	 * Affiche le sous-arbre et ses donn�es et caract�ristiques partag�es avec les autres arbres.
	 * Si details=false seules les informations partag�es sont affich�es, donc 2 arbres synchronis�s
	 * et stabilis�s g�n�reront exactement le m�me affichage avec cette m�thode.
	 * Si tmpId=true alors les identifiants temporaires sont affich�s.
	 * Si publicOnly=true alors les donn�es priv�es ne sont affich�es.
	 *
	 * @param distant distant dont l'abre doit �tre affich�, 0 pour afficher l'abre complet avec tous les distants
	 */
	virtual void print(std::ostringstream& out, DistantTreeProxy* distant, bool tmpId, bool publicOnly, int indentation) = 0;
};

#endif /* ABSTRACTBRANCHE_H_ */
