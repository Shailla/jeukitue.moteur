/*
 * Donnee.h
 *
 *  Created on: 9 mars 2013
 *      Author: vgdj7997
 */

#ifndef DONNEE_H_
#define DONNEE_H_

class Donnee {
	int revision;
public:
	Donnee();
	virtual ~Donnee();

	/**
	 * Indicates that the Donnee has changed by incrementing it's revision.
	 */
	void update();
};

#endif /* DONNEE_H_ */
