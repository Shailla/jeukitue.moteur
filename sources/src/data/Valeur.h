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

class Valeur : public Donnee {
	int _valeurId;
	string _valeurName;

	JktUtils::CGenRef _referenceGenerator;
public:
	Valeur(int valeurId, const string& valeurName);
	virtual ~Valeur();

	string getValeurName() const;
	int getValeurId() const;
};

#endif /* VALEUR_H_ */
