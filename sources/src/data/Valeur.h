/*
 * Valeur.h
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#ifndef VALEUR_H_
#define VALEUR_H_

#include <string>

#include "util/types/AnyData.h"
#include "data/Donnee.h"

#include "util/GenRef.h"

class AbstractBranche;

class Valeur : public Donnee {
	AbstractBranche* _parent;
	int _valeurId;
	std::string _valeurName;
	int _valeurTmpId;
public:
	Valeur(AbstractBranche* parent, int valeurId, const std::string& valeurName, DONNEE_TYPE valeurType, UPDATE_MODE updateMode, int valeurTmpId, int revision);
	virtual ~Valeur();

	std::vector<int> getBrancheId() const;
	std::vector<int> getBrancheIdOrTmpId() const;
	std::string getValeurName() const;
	int getValeurTmpId() const;
	int getValeurId() const;
	void setValeurId(int valeurId);
	virtual void setValeur(int revision, const jkt::AnyData& data) = 0;
	virtual jkt::AnyData getValeurData() const = 0;
	virtual std::string toString() const = 0;
	static bool highestId(const Valeur* left, const Valeur* right);
};

#endif /* VALEUR_H_ */
