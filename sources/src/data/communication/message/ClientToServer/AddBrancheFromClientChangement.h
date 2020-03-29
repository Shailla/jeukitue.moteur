/*
 * AddBrancheFromClientChangement.h
 *
 *  Created on: 11 mars 2013
 *      Author: Erwin
 */

#ifndef ADDBRANCHEFROMCLIENTCHANGEMENT_H_
#define ADDBRANCHEFROMCLIENTCHANGEMENT_H_

#include <sstream>
#include <vector>
#include <string>

#include "data/communication/message/Changement.h"

class MarqueurDistant;

class AddBrancheFromClientChangement: public Changement {
	std::vector<int> _parentBrancheId;
	int _brancheTmpId;
	int _revision;
	std::string _brancheName;

	void unserialize(std::istringstream& in);

public:
	AddBrancheFromClientChangement(std::istringstream& in);
	AddBrancheFromClientChangement(const std::vector<int>& parentBrancheId, int brancheTmpId, int revision, const std::string& brancheName);

	void update(MarqueurDistant* marqueur);
	void serialize(std::ostringstream& out) const;
	std::string toString() const;


	/* *******************************************
	 * Accessors
	 * ******************************************/

	const std::vector<int>& getParentBrancheId() const;
	int getBrancheTmpId() const;
	const std::string& getBrancheName() const;
	int getRevision() const;
	int getRootDistance() const;
};

#endif /* ADDBRANCHEFROMCLIENTCHANGEMENT_H_ */
