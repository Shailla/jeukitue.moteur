/*
 * Valeur.h
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#ifndef VALEURINT_H_
#define VALEURINT_H_

#include <string>

using namespace std;

#include "util/GenRef.h"
#include "data/Valeur.h"

class ValeurInt : public Valeur {
	int _valeur;
public:
	ValeurInt(int valeurId, const string& valeurName, int valeur);
	virtual ~ValeurInt();

	int getValeur() const;
	void updateValeur(int valeur);
};

#endif /* VALEUR_H_ */
