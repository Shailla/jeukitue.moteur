/*
 * AcceptAddValeurFromClientChangement.h
 *
 *  Created on: 11 mars 2013
 *      Author: Erwin
 */

#ifndef ACCEPTADDVALEURFROMCLIENTCHANGEMENT_H_
#define ACCEPTADDVALEURFROMCLIENTCHANGEMENT_H_

#include <sstream>
#include <vector>

#include "util/types/Data.h"
#include "data/MarqueurDistant.h"
#include "data/Branche.h"
#include "data/communication/message/Changement.h"

class AcceptAddValeurFromClientChangement : public Changement {
	std::vector<int> _brancheId;
	int _valeurTmpId;
	int _valeurId;
	int _revision;

	void unserialize(std::istringstream& in);

public:
	AcceptAddValeurFromClientChangement(std::istringstream& in);
	AcceptAddValeurFromClientChangement(const vector<int>& parentBrancheId, int brancheTmpId, int brancheId, int revision);
	~AcceptAddValeurFromClientChangement();

	void update(MarqueurDistant* marqueur);
	void serialize(std::ostringstream& out);
	std::string toString();


	/* *******************************************
	 * Accessors
	 * ******************************************/

	const std::vector<int>& getBrancheId() const;
	int getValeurTmpId() const;
	int getValeurId() const;
	int getRevision() const;
};

#endif /* ACCEPTADDVALEURFROMCLIENTCHANGEMENT_H_ */
