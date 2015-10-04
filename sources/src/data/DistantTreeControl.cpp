/*
 * DistantTreeControl.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#include <map>
#include <string>
#include <stdexcept>
#include <iostream>

using namespace std;

#include "data/ValeurInt.h"

#include "data/DistantTreeControl.h"

DistantTreeControl::DistantTreeControl() {
	_state = 0;
	_updateClientToServerDelay = 0;
	_updateServerToClientDelay = 0;
}

DistantTreeControl::~DistantTreeControl() {
}

TREE_STATE DistantTreeControl::getState() {
	if(_state) {
		return (TREE_STATE)_state->getValeur();
	}
	else {
		return STATE_NOT_READY;
	}
}

void DistantTreeControl::setState(TREE_STATE state) {
	if(_state) {
		return _state->updateValeur(state);
	}
}

int DistantTreeControl::getUpdateClientToServerDelay() {
	if(_updateClientToServerDelay) {
		return _updateClientToServerDelay->getValeur();
	}
	else {
		return -1;
	}
}

void DistantTreeControl::setUpdateClientToServerDelay(int updateClientToServerDelay) {
	if(_updateClientToServerDelay) {
		return _updateClientToServerDelay->updateValeur(updateClientToServerDelay);
	}
}

int DistantTreeControl::getUpdateServerToClientDelay() {
	if(_updateServerToClientDelay) {
		return _updateServerToClientDelay->getValeur();
	}
	else {
		return -1;
	}
}

void DistantTreeControl::setUpdateServerToClientDelay(int updateServerToClientDelay) {
	if(_updateServerToClientDelay) {
		return _updateServerToClientDelay->updateValeur(updateServerToClientDelay);
	}
}
