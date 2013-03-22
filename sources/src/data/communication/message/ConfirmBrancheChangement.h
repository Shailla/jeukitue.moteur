/*
 * ConfirmValeurChangement.h
 *
 *  Created on: 11 mars 2013
 *      Author: Erwin
 */

#ifndef CONFIRMBRANCHECHANGEMENT_H_
#define CONFIRMBRANCHECHANGEMENT_H_

#include <sstream>
#include <vector>

#include "util/types/Data.h"
#include "data/MarqueurDistant.h"
#include "data/Branche.h"
#include "data/communication/message/Changement.h"

class ConfirmBrancheChangement: public Changement {
	std::vector<int> _brancheId;
	int _revision;

	void unserialize(std::istringstream& in);

public:
	ConfirmBrancheChangement(std::istringstream& in);
	ConfirmBrancheChangement(const std::vector<int>& brancheId, int revision);
	~ConfirmBrancheChangement();

	void update(MarqueurDistant* marqueur);
	void serialize(std::ostringstream& out);
	std::string toString();


	/* *******************************************
	 * Accessors
	 * ******************************************/

	const std::vector<int>& getBrancheId() const;
	int getRevision() const;
};

#endif /* UPDATEVALEURCHANGEMENT_H_ */
