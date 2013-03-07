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

#include "data/Valeur.h"
#include "util/GenRef.h"

class Branche {
	int _brancheId;
	map<int, Branche*> _subBranches;
	map<int, Valeur*> _valeurs;

	JktUtils::CGenRef _brancheRefGenerator;
	JktUtils::CGenRef _valeurRefGenerator;
public:
	Branche(int brancheId);
	virtual ~Branche();

	Branche* getSubBranche(int brancheId) const;
	map<int, Branche*>& getSubBranches();
	map<int, Valeur*>& getValeurs();
	int getBrancheId() const;

	Branche* createSubBranche();
	Valeur* createValeurInt(int valeur);
};

#endif /* BRANCHE_H_ */
