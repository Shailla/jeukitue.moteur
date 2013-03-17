/*
 * Valeur.h
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#ifndef VALEUR_H_
#define VALEUR_H_

#include <string>

using namespace std;

#include "data/Donnee.h"

#include "util/GenRef.h"

class Branche;

class Valeur : public Donnee {
	Branche* _parent;
	int _valeurId;
	string _valeurName;
public:
	Valeur(Branche* parent, int valeurId, const string& valeurName);
	virtual ~Valeur();

	vector<int> getBrancheId() const;
	string getValeurName() const;
	int getValeurId() const;
};

#endif /* VALEUR_H_ */
