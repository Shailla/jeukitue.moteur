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
		ADD_BRANCHE_FROM_CLIENT_MESSAGE = 1,
		ADD_BRANCHE_FROM_SERVER_MESSAGE,
		ADD_VALEUR_FROM_SERVER_MESSAGE,
		UPDATE_VALEUR_FROM_SERVER_MESSAGE,
		CONFIRM_VALEUR_MESSAGE,
		CONFIRM_BRANCHE_MESSAGE,
		ADD_VALEUR_FROM_CLIENT_MESSAGE,

		ACCEPT_ADD_BRANCHE_FROM_CLIENT,
		ACCEPT_ADD_VALEUR_FROM_CLIENT
	};

	enum ValeurType {
		INT_VALEUR_TYPE = 1,
		FLOAT_VALEUR_TYPE,
		FLOAT3_VALEUR_TYPE,
		FLOAT4_VALEUR_TYPE,
		STRING_VALEUR_TYPE
	};

	string _dataType;

	virtual void unserialize(std::istringstream& in) = 0;

public:
	Changement(const string& dataType);
	virtual ~Changement();

	virtual void update(MarqueurDistant* marqueur) = 0;
	virtual void serialize(std::ostringstream& out) = 0;

	virtual std::string toString();
};

#endif /* _CHANGEMENT_H_ */
