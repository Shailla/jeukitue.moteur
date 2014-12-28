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

#include "util/types/AnyData.h"
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
	vector<int> getBrancheIdOrTmpId() const;
	string getValeurName() const;
	int getValeurTmpId() const;
	int getValeurId() const;
	void setValeurId(int valeurId);
	virtual void setValeur(int revision, const JktUtils::AnyData& data) = 0;
	virtual JktUtils::AnyData getValeurData() const = 0;
	virtual std::string toString() const = 0;
	static bool highestId(const Valeur* left, const Valeur* right);
};

#endif /* VALEUR_H_ */
