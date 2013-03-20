/*
 * ConfirmValeurChangement.h
 *
 *  Created on: 11 mars 2013
 *      Author: Erwin
 */

#ifndef CONFIRMVALEURCHANGEMENT_H_
#define CONFIRMVALEURCHANGEMENT_H_

#include <sstream>
#include <vector>

#include "util/types/Data.h"
#include "data/MarqueurDistant.h"
#include "data/Branche.h"
#include "data/communication/message/Changement.h"

class ConfirmValeurChangement: public Changement {
	std::vector<int> _brancheId;
	int _valeurId;
	int _revision;

	void unserialize(std::istringstream& in);

public:
	ConfirmValeurChangement(std::istringstream& in);
	ConfirmValeurChangement(Valeur* valeur);
	~ConfirmValeurChangement();

	void update(MarqueurDistant* marqueur);
	void serialize(std::ostringstream& out);


	/* *******************************************
	 * Accessors
	 * ******************************************/

	const std::vector<int>& getBrancheId() const;
	int getValeurId() const;
	int getRevision() const;
};

#endif /* UPDATEVALEURCHANGEMENT_H_ */
