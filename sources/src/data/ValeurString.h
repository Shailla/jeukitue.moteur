/*
 * ValeurString.h
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#ifndef VALEURSTRING_H_
#define VALEURSTRING_H_

#include <string>

#include "util/GenRef.h"
#include "data/Valeur.h"

class Branche;

class ValeurString : public Valeur {
	std::string _valeur;
public:
	ValeurString(AbstractBranche* parent, int valeurId, const string& valeurName, DONNEE_TYPE valeurType, UPDATE_MODE updateMode, int tmpRef, int revision, const std::string& valeur);
	virtual ~ValeurString();

	const std::string& getValeur() const;
	jkt::AnyData getValeurData() const;

	void setValeur(int revision, const std::string& valeur);
	void setValeur(int revision, const jkt::AnyData& data);

	void updateValeur(const std::string& valeur);

	std::string toString() const;
};

#endif /* VALEURINT_H_ */
