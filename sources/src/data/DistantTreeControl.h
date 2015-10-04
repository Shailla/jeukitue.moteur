/*
 * DistantTreeControl.h
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#ifndef DISTANTTREECONTROL_H_
#define DISTANTTREECONTROL_H_

#include "data/TreeConstants.h"

#include <string>

class ValeurInt;

class DistantTreeControl {
	ValeurInt* _state;
	ValeurInt* _updateClientToServerDelay;
	ValeurInt* _updateServerToClientDelay;

public:
	DistantTreeControl();
	virtual ~DistantTreeControl();

	// State
	TREE_STATE getState();
	void setState(TREE_STATE state);

	// Update client to server delay
	int getUpdateClientToServerDelay();
	void setUpdateClientToServerDelay(int updateClientToServerDelay);

	// Update server to client delay
	int getUpdateServerToClientDelay();
	void setUpdateServerToClientDelay(int updateServerToClientDelay);
};

#endif /* DISTANTTREECONTROL_H_ */
