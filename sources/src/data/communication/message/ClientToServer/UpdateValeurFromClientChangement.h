/*
 * UpdateValeurFromClientChangement.h
 *
 *  Created on: 11 mars 2013
 *      Author: Erwin
 */

#ifndef UPDATEVALEURFROMCLIENTCHANGEMENT_H_
#define UPDATEVALEURFROMCLIENTCHANGEMENT_H_

#include <sstream>
#include <vector>

#include "util/types/Data.h"
#include "data/MarqueurDistant.h"
#include "data/Branche.h"
#include "data/communication/message/Changement.h"

class UpdateValeurFromClientChangement: public Changement {
	std::vector<int> _brancheId;
	int _valeurId;
	int _revision;
	JktUtils::Data* _valeur;

	void unserialize(std::istringstream& in);

public:
	UpdateValeurFromClientChangement(std::istringstream& in);
	UpdateValeurFromClientChangement(const std::vector<int>& brancheId, int valeurId, int revision, JktUtils::Data* valeur);
	~UpdateValeurFromClientChangement();

	void update(MarqueurDistant* marqueur);
	void serialize(std::ostringstream& out);
	std::string toString();


	/* *******************************************
	 * Accessors
	 * ******************************************/

	const std::vector<int>& getBrancheId() const;
	int getValeurId() const;
	int getRevision() const;
	JktUtils::Data* getValeur() const;
};

#endif /* UPDATEVALEURFROMCLIENTCHANGEMENT_H_ */
