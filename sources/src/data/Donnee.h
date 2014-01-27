/*
 * Donnee.h
 *
 *  Created on: 9 mars 2013
 *      Author: vgdj7997
 */

#ifndef DONNEE_H_
#define DONNEE_H_

#include <map>

class DistantTreeProxy;
class MarqueurDistant;
class Branche;
class Valeur;

class Donnee {
	/**
	 * Revision number of the data, incremented each time the data changes.
	 */
	int _revision;
	std::map<DistantTreeProxy*, MarqueurDistant*> _marqueurs;

protected:

	/**
	 * Indicates that the Donnee has changed by incrementing it's revision.
	 */
	void update();

public:
	Donnee(int revision);
	virtual ~Donnee();

	MarqueurDistant* getMarqueur(DistantTreeProxy* distant);
	MarqueurDistant* addMarqueur(DistantTreeProxy* distant, int donneeTmpId);

	/**
	 * Get the revision number of the data.
	 */
	int getRevision() const;
	void setRevision(int revision);

	static bool highestRevision(const Donnee* left, const Donnee* right);
};

#endif /* DONNEE_H_ */
