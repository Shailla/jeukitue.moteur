/*
 * AddBrancheChangement.h
 *
 *  Created on: 11 mars 2013
 *      Author: Erwin
 */

#ifndef UPDATEVALEURCHANGEMENT_H_
#define UPDATEVALEURCHANGEMENT_H_

#include <sstream>
#include <vector>

#include "util/types/Data.h"
#include "data/MarqueurDistant.h"
#include "data/Branche.h"
#include "data/communication/message/Changement.h"

class UpdateValeurChangement: public Changement {
	std::vector<int> _brancheId;
	int _valeurId;
	int _revision;
	JktUtils::Data* _valeur;

	void unserialize(std::istringstream& in);

public:
	UpdateValeurChangement(std::istringstream& in);
	UpdateValeurChangement(Valeur* valeur);
	~UpdateValeurChangement();

	void update(MarqueurDistant* marqueur);
	void serialize(std::ostringstream& out);


	/* *******************************************
	 * Accessors
	 * ******************************************/

	const std::vector<int>& getBrancheId() const;
	int getValeurId() const;
	int getRevision() const;
	JktUtils::Data* getValeur() const;
};

#endif /* UPDATEVALEURCHANGEMENT_H_ */
