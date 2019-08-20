/*
 * AcceptAddBrancheFromClientChangement.h
 *
 *  Created on: 11 mars 2013
 *      Author: Erwin
 */

#ifndef ACCEPTADDBRANCHEFROMCLIENTCHANGEMENT_H_
#define ACCEPTADDBRANCHEFROMCLIENTCHANGEMENT_H_

#include <sstream>
#include <vector>

#include "includes.h"

#include "data/communication/message/Changement.h"

class AcceptAddBrancheFromClientChangement: public Changement {
	std::vector<int> _parentBrancheId;
	int _brancheTmpId;
	int _brancheId;
	int _revision;

	void unserialize(std::istringstream& in);

public:
	AcceptAddBrancheFromClientChangement(std::istringstream& in);
	AcceptAddBrancheFromClientChangement(const std::vector<int>& parentBrancheId, int brancheTmpId, int brancheId, int revision);
	~AcceptAddBrancheFromClientChangement();

	void update(MarqueurDistant* marqueur);
	void serialize(std::ostringstream& out) const;
	std::string toString() const;


	/* *******************************************
	 * Accessors
	 * ******************************************/

	const std::vector<int>& getParentBrancheId() const;
	int getBrancheTmpId() const;
	int getBrancheId() const;
	int getRevision() const;
	int getRootDistance() const;
};

#endif /* ACCEPTADDBRANCHEFROMCLIENTCHANGEMENT_H_ */
