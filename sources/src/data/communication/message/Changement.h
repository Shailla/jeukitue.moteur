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
		UPDATE_VALEUR_FROM_CLIENT_MESSAGE,

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

	enum ChangementPriority {
		PRIORITY_UpdateValeurFromServerChangement = 0,
		PRIORITY_UpdateValeurFromClientChangement,
		PRIORITY_ConfirmValeurChangement,
		PRIORITY_AcceptAddValeurFromClientChangement,
		PRIORITY_AddValeurFromServerChangement,
		PRIORITY_AddValeurFromClientChangement,
		PRIORITY_ConfirmBrancheChangement,
		PRIORITY_AcceptAddBrancheFromClientChangement,
		PRIORITY_AddBrancheFromServerChangement,
		PRIORITY_AddBrancheFromClientChangement
	};

	string _dataType;

	virtual void unserialize(std::istringstream& in) = 0;

protected:
	int _majorPriority;
	int _minorPriority;

public:
	Changement(const std::string& dataType, int majorPriority, int minorPriority);
	virtual ~Changement();

	virtual void update(MarqueurDistant* marqueur) = 0;
	virtual void serialize(std::ostringstream& out) const = 0;

	/**
	 * Return a number which indicates the absolute priority of this changement, the more it's priority is high, the more this changement
	 * should be executed before an other one.
	 * For example, a change on a branch has an higher priority tan a change on a value.
	 */
	int getMajorPriority() const;

	/**
	 * Return a number which indicates the absolute priority of this changement, the more it's priority is high, the more this changement
	 * should be executed before an other one.
	 * For example, a change on a branch has an higher priority tan a change on a value.
	 */
	int getMinorPriority() const;

	/** Return the number of parent branches between the value or branche modified by this changements and the root branche of the tree. */
	virtual int getRootDistance() const = 0;

	/**
	 * left is higher priority than right if :
	 *    - left is a branch when right is a value
	 *    - left and right are branches but left is nearest of the root branch of the tree
	 * Prioritary is unknown if :
	 *    - left and right are values
	 *    - left and right are branches but they have the same distance with the root branch of the tree
	 */
	static bool highestPriority(const Changement* left, const Changement* right);

	virtual std::string toString() const;
};

#endif /* _CHANGEMENT_H_ */
