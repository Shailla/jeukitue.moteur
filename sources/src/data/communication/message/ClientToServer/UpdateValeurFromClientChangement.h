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

#include "data/MarqueurDistant.h"
#include "data/Branche.h"
#include "data/communication/message/Changement.h"

class UpdateValeurFromClientChangement: public Changement {
	std::vector<int> _parentBrancheId;
	int _valeurId;
	int _revision;
	JktUtils::AnyData _valeur;

	void unserialize(std::istringstream& in);

public:
	UpdateValeurFromClientChangement(std::istringstream& in);
	UpdateValeurFromClientChangement(const std::vector<int>& brancheId, int valeurId, int revision, JktUtils::AnyData valeur);
	~UpdateValeurFromClientChangement();

	void update(MarqueurDistant* marqueur);
	void serialize(std::ostringstream& out) const;
	std::string toString() const;


	/* *******************************************
	 * Accessors
	 * ******************************************/

	const std::vector<int>& getParentBrancheId() const;
	int getValeurId() const;
	int getRevision() const;
	JktUtils::AnyData getValeur() const;
	int getRootDistance() const;
};

#endif /* UPDATEVALEURFROMCLIENTCHANGEMENT_H_ */
