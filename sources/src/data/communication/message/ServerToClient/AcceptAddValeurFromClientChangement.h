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

#include "data/MarqueurDistant.h"
#include "data/Branche.h"
#include "data/communication/message/Changement.h"

class AcceptAddValeurFromClientChangement : public Changement {
	std::vector<int> _parentBrancheId;
	int _valeurTmpId;
	int _valeurId;
	int _revision;

	void unserialize(std::istringstream& in);

public:
	AcceptAddValeurFromClientChangement(std::istringstream& in);
	AcceptAddValeurFromClientChangement(const std::vector<int>& brancheId, int valeurTmpId, int valeurId, int revision);
	~AcceptAddValeurFromClientChangement();

	void update(MarqueurDistant* marqueur);
	void serialize(std::ostringstream& out) const;
	std::string toString() const;


	/* *******************************************
	 * Accessors
	 * ******************************************/

	const std::vector<int>& getParentBrancheId() const;
	int getValeurTmpId() const;
	int getValeurId() const;
	int getRevision() const;
	int getRootDistance() const;
};

#endif /* ACCEPTADDVALEURFROMCLIENTCHANGEMENT_H_ */
