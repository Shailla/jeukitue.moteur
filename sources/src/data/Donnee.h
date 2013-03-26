/*
 * Donnee.h
 *
 *  Created on: 9 mars 2013
 *      Author: vgdj7997
 */

#ifndef DONNEE_H_
#define DONNEE_H_

#include <map>

class Distant;
class MarqueurDistant;

class Donnee {
	/**
	 * Revision number of the data, incremented each time the data changes.
	 */
	int _revision;
	std::map<Distant*, MarqueurDistant*> _marqueurs;

public:
	Donnee(int revision);
	virtual ~Donnee();

	MarqueurDistant* getMarqueur(Distant* distant);
	MarqueurDistant* addMarqueur(Distant* distant, int donneeTmpId);

	/**
	 * Indicates that the Donnee has changed by incrementing it's revision.
	 */
	void update();

	/**
	 * Get the revision number of the data.
	 */
	int getRevision() const;
	void setRevision(int revision);
};

#endif /* DONNEE_H_ */
