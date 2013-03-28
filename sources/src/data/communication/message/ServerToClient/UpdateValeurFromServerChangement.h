/*
 * UpdateValeurFromServerChangement.h
 *
 *  Created on: 11 mars 2013
 *      Author: Erwin
 */

#ifndef UPDATEVALEURFROMSERVERCHANGEMENT_H_
#define UPDATEVALEURFROMSERVERCHANGEMENT_H_

#include <sstream>
#include <vector>

#include "util/types/Data.h"
#include "data/communication/message/Changement.h"

class MarqueurDistant;
class Valeur;

class UpdateValeurFromServerChangement: public Changement {
	std::vector<int> _brancheId;
	int _valeurId;
	int _revision;
	JktUtils::Data* _valeur;

	void unserialize(std::istringstream& in);

public:
	UpdateValeurFromServerChangement(std::istringstream& in);
	UpdateValeurFromServerChangement(const Valeur* valeur);
	~UpdateValeurFromServerChangement();

	void update(MarqueurDistant* marqueur);
	void serialize(std::ostringstream& out);
	std::string toString();

	/* *******************************************
	 * Accessors
	 * ******************************************/

	const std::vector<int>& getBrancheId() const;
	int getValeurId() const;
	int getRevision() const;
	const JktUtils::Data* getValeur() const;
};

#endif /* UPDATEVALEURFROMSERVERCHANGEMENT_H_ */
