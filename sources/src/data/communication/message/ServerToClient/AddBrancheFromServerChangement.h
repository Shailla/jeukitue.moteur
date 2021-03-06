/*
 * AddBrancheFromServerChangement.h
 *
 *  Created on: 11 mars 2013
 *      Author: Erwin
 */

#ifndef ADDBRANCHEFROMSERVERCHANGEMENT_H_
#define ADDBRANCHEFROMSERVERCHANGEMENT_H_

#include <sstream>
#include <vector>
#include <string>

#include "data/communication/message/Changement.h"

class Branche;
class MarqueurDistant;

class AddBrancheFromServerChangement: public Changement {
	std::vector<int> _parentBrancheId;
	int _brancheId;
	int _revision;
	std::string _brancheName;

	void unserialize(std::istringstream& in);

public:
	AddBrancheFromServerChangement(std::istringstream& in);
	AddBrancheFromServerChangement(const std::vector<int>& parentBrancheId, int brancheId, int revision, const std::string& brancheName);

	void update(MarqueurDistant* marqueur);
	void serialize(std::ostringstream& out) const;
	std::string toString() const;


	/* *******************************************
	 * Accessors
	 * ******************************************/

	int getBrancheId() const;
	const std::vector<int>& getParentBrancheId() const;
	const std::string& getBrancheName() const;
	int getRevision() const;
	int getRootDistance() const;
};

#endif /* ADDBRANCHEFROMSERVERCHANGEMENT_H_ */
