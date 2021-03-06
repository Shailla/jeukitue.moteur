/*
 * ValeurInt.h
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

class Branche;

class ValeurInt : public Valeur {
	int _valeur;
public:
	ValeurInt(Branche* parent, int valeurId, const string& valeurName, int tmpRef, int revision, int valeur);
	virtual ~ValeurInt();

	int getValeur() const;
	JktUtils::Data* getValeurData() const;

	void setValeur(int revision, int valeur);
	void setValeur(int revision, const JktUtils::Data& data);

	void updateValeur(const JktUtils::Data* data);
	void updateValeur(int valeur);

	std::string toString() const;
};

#endif /* VALEURINT_H_ */
