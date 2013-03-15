/*
 * AddBrancheChangement.h
 *
 *  Created on: 11 mars 2013
 *      Author: Erwin
 */

#ifndef ADDBRANCHECHANGEMENT_H_
#define ADDBRANCHECHANGEMENT_H_

#include <sstream>
#include <vector>

#include "data/MarqueurClient.h"
#include "data/Branche.h"
#include "data/communication/message/Changement.h"

class AddBrancheChangement: public Changement {
	std::vector<int> _parentBrancheId;
	int _brancheId;
	int _revision;
	string _brancheName;

	void unserialize(std::istringstream& in);

public:
	AddBrancheChangement(std::istringstream& in);
	AddBrancheChangement(const std::vector<int>& parentBrancheId, int brancheId, int revision, const string& brancheName);

	void update(MarqueurClient* marqueur);
	void change(DataTree* tree);

	void serialize(std::ostringstream& out);
};

#endif /* ADDBRANCHECHANGEMENT_H_ */
