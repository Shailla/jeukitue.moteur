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

using namespace std;

#include "util/types/Data.h"
#include "data/Donnee.h"
#include "data/Valeur.h"
#include "util/GenRef.h"

class Branche : public Donnee {
	Branche* _parent;
	int _brancheId;
	int _brancheTmpId;
	string _brancheName;

	map<int, Branche*> _subBranches;
	map<int, Valeur*> _valeurs;

	JktUtils::CGenRef _brancheTmpRefGenerator;
	JktUtils::CGenRef _brancheRefGenerator;
	JktUtils::CGenRef _valeurRefGenerator;
	JktUtils::CGenRef _valeurTmpRefGenerator;
public:
	Branche(Branche* parent, int brancheId, const string& brancheName, int revision, int tmpId);
	virtual ~Branche();

	Branche* getSubBranche(int brancheId) const;
	map<int, Branche*>& getSubBranches();
	map<int, Valeur*>& getValeurs();
	Valeur* getValeur(int valeurId);
	string getBrancheName() const;
	int getBrancheId() const;
	int getBrancheTmpId() const;
	vector<int> getParentBrancheId(void) const;
	void getBrancheFullId(vector<int>& id) const;
	vector<int> getBrancheFullId() const;

	/** Crée une nouvelle branche et lui attribue un identifiant temporaire */
	Branche* createSubBrancheForClient(const string& brancheName);

	/** Crée une nouvelle branche et lui attribue un identifiant */
	Branche* createSubBrancheForServer(const string& brancheName);

	/** Ajoute une branche qui a déjà un identifiant car elle a par exemple été créée sur le serveur puis diffusée */
	Branche* addSubBranche(int brancheId, const std::string& brancheName, int brancheRevision);

	/** Crée une nouvelle valeur entière et lui attribue un identifiant temporaire */
	Valeur* createValeurIntForClient(const string& valeurName, int valeur);

	/** Crée une nouvelle valeur entière et lui attribue un identifiant */
	Valeur* createValeurIntForServeur(const string& valeurName, int valeur);

	/** Ajoute une valeur entière qui a déjà un identifiant car elle a par exemple été créée sur le serveur puis diffusée */
	Valeur* addValeurInt(int valeurId, const string& valeurName, int valeurRevision, JktUtils::Data* valeur);
};

#endif /* BRANCHE_H_ */
