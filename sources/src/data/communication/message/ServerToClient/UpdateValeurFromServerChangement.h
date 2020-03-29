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

#include "includes.h"

#include "data/communication/message/Changement.h"

class UpdateValeurFromServerChangement: public Changement {
	std::vector<int> _parentBrancheId;
	int _valeurId;
	int _revision;
	jkt::AnyData _valeur;

	void unserialize(std::istringstream& in);

public:
	UpdateValeurFromServerChangement(std::istringstream& in);
	UpdateValeurFromServerChangement(const std::vector<int>& brancheId, int valeurId, int revision, jkt::AnyData valeur);
	~UpdateValeurFromServerChangement();

	void update(MarqueurDistant* marqueur);
	void serialize(std::ostringstream& out) const;
	std::string toString() const;

	/* *******************************************
	 * Accessors
	 * ******************************************/

	const std::vector<int>& getParentBrancheId() const;
	int getValeurId() const;
	int getRevision() const;
	const jkt::AnyData getValeur() const;
	int getRootDistance() const;
};

#endif /* UPDATEVALEURFROMSERVERCHANGEMENT_H_ */
