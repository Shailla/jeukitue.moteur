/*
 * ValeurFloat.h
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#ifndef VALEURFLOAT_H_
#define VALEURFLOAT_H_

#include <string>

using namespace std;

#include "util/GenRef.h"
#include "data/Valeur.h"

class Branche;

class ValeurFloat : public Valeur {
	float _valeur;
public:
	ValeurFloat(AbstractBranche* parent, int valeurId, const string& valeurName, DONNEE_TYPE valeurType, UPDATE_MODE updateMode, int tmpRef, int revision, float valeur);
	virtual ~ValeurFloat();

	float getValeur() const;
	JktUtils::AnyData getValeurData() const;

	void setValeur(int revision, float valeur);
	void setValeur(int revision, const JktUtils::AnyData& data);

	void updateValeur(float valeur);

	std::string toString() const;
};

#endif /* VALEURINT_H_ */
