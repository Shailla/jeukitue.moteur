/*
 * AddBrancheChangement.h
 *
 *  Created on: 11 mars 2013
 *      Author: Erwin
 */

#ifndef ADDVALEURCHANGEMENT_H_
#define ADDVALEURCHANGEMENT_H_

#include <sstream>
#include <vector>

#include "data/MarqueurClient.h"
#include "data/Branche.h"
#include "data/communication/message/Changement.h"

class AddValeurChangement: public Changement {
	std::vector<int> _brancheId;
	int _valeurId;
	string _valeurName;
	int _revision;
	int _valeur;

	void unserialize(std::istringstream& in);

public:
	AddValeurChangement(std::istringstream& in);
	AddValeurChangement(const std::vector<int>& brancheId, int valeurId, int revision, const string& valeurName, int valeur);

	void update(MarqueurClient* marqueur);
	void serialize(std::ostringstream& out);


	/* *******************************************
	 * Accessors
	 * ******************************************/

	const std::vector<int>& getBrancheId() const;
	int getValeurId() const;
	const string& getValeurName() const;
	int getRevision() const;
	int getValeur() const;
};

#endif /* ADDBRANCHECHANGEMENT_H_ */
