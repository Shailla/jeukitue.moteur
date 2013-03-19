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
class MarqueurDistant;

class Changement {
	friend DataSerializer;
protected:
	enum MessageType {
		ADD_BRANCHE_MESSAGE = 1,
		ADD_VALEUR_MESSAGE,
		UPDATE_VALEUR_MESSAGE
	};

	enum ValeurType {
		INT_VALEUR_TYPE = 1,
		FLOAT_VALEUR_TYPE,
		FLOAT3_VALEUR_TYPE,
		FLOAT4_VALEUR_TYPE,
		STRING_VALEUR_TYPE
	};

	virtual void unserialize(std::istringstream& in) = 0;

public:
	Changement();
	virtual ~Changement();

	virtual void update(MarqueurDistant* marqueur) = 0;
	virtual void serialize(std::ostringstream& out) = 0;
};

#endif /* _CHANGEMENT_H_ */
