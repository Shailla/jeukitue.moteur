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

#include "util/types/Data.h"
#include "data/Donnee.h"

#include "util/GenRef.h"

class Branche;

class Valeur : public Donnee {
	Branche* _parent;
	int _valeurId;
	string _valeurName;
	int _valeurTmpId;
public:
	Valeur(Branche* parent, int valeurId, const string& valeurName, int valeurTmpId, int revision);
	virtual ~Valeur();

	vector<int> getBrancheId() const;
	string getValeurName() const;
	int getValeurTmpId() const;
	int getValeurId() const;
	void setValeurId(int valeurId);
	virtual void setValeur(int revision, const JktUtils::Data& data) = 0;
	virtual JktUtils::Data* getValeurData() const = 0;
};

#endif /* VALEUR_H_ */
