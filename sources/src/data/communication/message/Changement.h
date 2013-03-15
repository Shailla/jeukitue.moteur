/*
 * Changement.h
 *
 *  Created on: 11 mars 2013
 *      Author: vgdj7997
 */

#ifndef _CHANGEMENT_H_
#define _CHANGEMENT_H_

#include <sstream>

class ServeurDataTree;
class DataSerializer;
class MarqueurClient;

class Changement {
	friend DataSerializer;
protected:
	enum MessageType {
		ADD_DATA_MESSAGE = 1
	};

	virtual void unserialize(std::istringstream& in) = 0;

public:
	Changement();
	virtual ~Changement();

	virtual void update(MarqueurClient* marqueur) = 0;
	virtual void change(ServeurDataTree* tree) = 0;

	virtual void serialize(std::ostringstream& out) = 0;
};

#endif /* _CHANGEMENT_H_ */
