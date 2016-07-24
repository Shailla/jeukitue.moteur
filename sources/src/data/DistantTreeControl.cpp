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

#include "data/Branche.h"
#include "data/ValeurInt.h"
#include "main/Cfg.h"
#include "data/DataTreeUtils.h"

#include "data/DistantTreeControl.h"

extern CCfg Config;

DistantTreeControl::DistantTreeControl(DistantTreeProxy* distant) {
	_distant = distant;
	_state = 0;
	_updateClientToServerDelay = 0;
	_updateServerToClientDelay = 0;
}

DistantTreeControl::~DistantTreeControl() {
}

/**
 * Formatte un arbre de données vierge en créant les branches et hiérarchies suivantes :
 * [1]			"vid"
 * [1,1]		  "tree-control"
 * [1,1,1]		    "tree-state" int
 * [1,1,1]		    "tree-update-delay" int
 */
void DistantTreeControl::initVid(DataTree *tree, DistantTreeProxy* distant) {
	vector<string> treeControlPath;
	treeControlPath.push_back(DataTreeUtils::TREE_VID_BRANCHE_NAME);
	treeControlPath.push_back(DataTreeUtils::TREE_CONTROL_BRANCHE_NAME);
	PrivateBranche* treeControlBr = (PrivateBranche*)tree->getBranche(distant, treeControlPath);

	distant->getControl().create(treeControlBr);
}

void DistantTreeControl::create(Branche* branche) {
	_state = (ValeurInt*)branche->createValeurForServeur(_distant, ANY, "tree-state", 0, jkt::AnyData(TREE_STATE::STATE_NOT_READY));
	_updateClientToServerDelay = (ValeurInt*)branche->createValeurForServeur(_distant, ANY, "tree-update-clientToServer-delay", 0, jkt::AnyData(Config.Reseau.getTreeUpdateClientToServerDelay()));
	_updateServerToClientDelay = (ValeurInt*)branche->createValeurForServeur(_distant, ANY, "tree-update-serverToClient-delay", 0, jkt::AnyData(Config.Reseau.getTreeUpdateServerToClientDelay()));
}

void DistantTreeControl::attach(Branche* branche) {
	_state = (ValeurInt*)branche->getValeurByName(0, "tree-state");
	_updateClientToServerDelay = (ValeurInt*)branche->createValeurForServeur(_distant, ANY, "tree-update-clientToServer-delay", 0, jkt::AnyData(Config.Reseau.getTreeUpdateClientToServerDelay()));
	_updateServerToClientDelay = (ValeurInt*)branche->createValeurForServeur(_distant, ANY, "tree-update-serverToClient-delay", 0, jkt::AnyData(Config.Reseau.getTreeUpdateServerToClientDelay()));
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
		return 200;
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
		return 200;
	}
}

void DistantTreeControl::setUpdateServerToClientDelay(int updateServerToClientDelay) {
	if(_updateServerToClientDelay) {
		return _updateServerToClientDelay->updateValeur(updateServerToClientDelay);
	}
}
