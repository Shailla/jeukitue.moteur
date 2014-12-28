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

#include "data/communication/message/Changement.h"

class MarqueurDistant;

class AddValeurFromServerChangement: public Changement {
	std::vector<int> _parentBrancheId;
	int _valeurId;
	std::string _valeurName;
	int _revision;
	JktUtils::AnyData _valeur;

	void unserialize(std::istringstream& in);

public:
	AddValeurFromServerChangement(std::istringstream& in);
	AddValeurFromServerChangement(const std::vector<int>& brancheId, int valeurId, int revision, const std::string& valeurName, JktUtils::AnyData valeur);
	~AddValeurFromServerChangement();

	void update(MarqueurDistant* marqueur);
	void serialize(std::ostringstream& out) const;
	std::string toString() const;


	/* *******************************************
	 * Accessors
	 * ******************************************/

	const std::vector<int>& getParentBrancheId() const;
	int getValeurId() const;
	const std::string& getValeurName() const;
	int getRevision() const;
	const JktUtils::AnyData getValeur() const;
	int getRootDistance() const;
};

#endif /* ADDBRANCHECHANGEMENT_H_ */
