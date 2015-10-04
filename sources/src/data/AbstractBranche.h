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

	JktUtils::CGenRef _brancheTmpRefGenerator;
	JktUtils::CGenRef _brancheRefGenerator;
	JktUtils::CGenRef _valeurRefGenerator;
	JktUtils::CGenRef _valeurTmpRefGenerator;

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

	virtual Branche* getSubBrancheByIdOrDistantTmpId(DistantTreeProxy* distant, int brancheId) throw(NotExistingBrancheException) = 0;
	virtual Branche* getSubBrancheByDistantTmpId(DistantTreeProxy* distant, int brancheId) throw(NotExistingBrancheException) = 0;

	virtual Valeur* getValeurByDistantTmpId(DistantTreeProxy* distant, int valeurTmpId) throw(NotExistingValeurException) = 0;

	/**
	 * Affiche le sous-arbre et ses donn�es et caract�ristiques partag�es avec les autres arbres.
	 * 2 arbres synchronis�s et stabilis�s g�n�reront exactement le m�me affichage avec cette m�thode.
	 */
	std::string print(int indentation, bool details);

	/**
	 * Affiche le sous-arbre et ses donn�es et caract�ristiques partag�es avec les autres arbres.
	 * Si details=false seules les informations partag�es sont affich�es, donc 2 arbres synchronis�s
	 * et stabilis�s g�n�reront exactement le m�me affichage avec cette m�thode.
	 * Si details=true alors plus de caract�risques sont affich�es.
	 */
	virtual void print(std::ostringstream& out, bool details, int indentation) = 0;
};

#endif /* ABSTRACTBRANCHE_H_ */
