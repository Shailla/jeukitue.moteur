/*
 * AddValeurFromClientChangement.h
 *
 *  Created on: 11 mars 2013
 *      Author: Erwin
 */

#ifndef ADDVALEURFROMCLIENTCHANGEMENT_H_
#define ADDVALEURFROMCLIENTCHANGEMENT_H_

#include <sstream>
#include <vector>

#include "util/types/Data.h"
#include "data/MarqueurDistant.h"
#include "data/Branche.h"
#include "data/communication/message/Changement.h"

class AddValeurFromClientChangement: public Changement {
	std::vector<int> _brancheId;
	int _valeurTmpId;
	string _valeurName;
	int _revision;
	JktUtils::Data* _valeur;

	void unserialize(std::istringstream& in);

public:
	AddValeurFromClientChangement(std::istringstream& in);
	AddValeurFromClientChangement(const std::vector<int>& brancheId, int valeurTmpId, int revision, const string& valeurName, JktUtils::Data* valeur);
	~AddValeurFromClientChangement();

	void update(MarqueurDistant* marqueur);
	void serialize(std::ostringstream& out);
	std::string toString();


	/* *******************************************
	 * Accessors
	 * ******************************************/

	const std::vector<int>& getBrancheId() const;
	int getValeurTmpId() const;
	const string& getValeurName() const;
	int getRevision() const;
	JktUtils::Data* getValeur() const;
};

#endif /* ADDVALEURFROMCLIENTCHANGEMENT_H_ */
