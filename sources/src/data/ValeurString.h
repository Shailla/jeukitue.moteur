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
	ValeurString(Branche* parent, int valeurId, const string& valeurName, int tmpRef, int revision, const std::string& valeur);
	virtual ~ValeurString();

	const std::string& getValeur() const;
	JktUtils::Data* getValeurData() const;

	void setValeur(int revision, const std::string& valeur);
	void setValeur(int revision, const JktUtils::Data& data);

	void updateValeur(const JktUtils::Data* data);
	void updateValeur(const std::string& valeur);

	std::string toString() const;
};

#endif /* VALEURINT_H_ */
