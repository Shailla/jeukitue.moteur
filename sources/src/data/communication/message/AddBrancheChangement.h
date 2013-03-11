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
	std::vector<int> _brancheId;
	int _revision;

	void unserialize(IMessageStream& in);

public:
	AddBrancheChangement(IMessageStream& in);
	AddBrancheChangement(std::vector<int>& brancheId, int revision);

	void update(MarqueurClient* marqueur);
	void change(DataTree* tree);

	void serialize(OMessageStream& out);
};

#endif /* ADDBRANCHECHANGEMENT_H_ */
