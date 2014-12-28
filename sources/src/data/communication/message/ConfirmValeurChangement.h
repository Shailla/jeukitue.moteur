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

#include "data/communication/message/Changement.h"

class MarqueurDistant;

class ConfirmValeurChangement: public Changement {
	std::vector<int> _brancheId;
	int _valeurId;
	int _revision;

	void unserialize(std::istringstream& in);

public:
	ConfirmValeurChangement(std::istringstream& in);
	ConfirmValeurChangement(const std::vector<int>& brancheId, int valeurId, int revision);
	~ConfirmValeurChangement();

	void update(MarqueurDistant* marqueur);
	void serialize(std::ostringstream& out) const;
	std::string toString() const;


	/* *******************************************
	 * Accessors
	 * ******************************************/

	const std::vector<int>& getBrancheId() const;
	int getValeurId() const;
	int getRevision() const;
	int getRootDistance() const;
};

#endif /* UPDATEVALEURCHANGEMENT_H_ */
