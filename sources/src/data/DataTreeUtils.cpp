/*
 * DataTreeUtils.cpp
 *
 *  Created on: 19 déc. 2014
 *      Author: VGDJ7997
 */

#include "main/Cfg.h"
#include "data/TreeConstants.h"

#include "data/DataTreeUtils.h"

const char* DataTreeUtils::TREE_VID_BRANCHE_NAME = "vid";
const char* DataTreeUtils::TREE_CONTROL_BRANCHE_NAME = "tree-control";

extern CCfg Config;

DataTreeUtils::DataTreeUtils() {
}

DataTreeUtils::~DataTreeUtils() {
}

/**
 * Formatte un arbre de données vierge en créant les branches et hiérarchies suivantes :
 * [1]			"vid"
 * [1,1]		  "tree-control"
 * [1,1,1]		    "tree-state" int
 * [1,1,1]		    "tree-update-delay" int
 * [2]  		"map"
 * [2,1]		  "info"
 * [2,1]-1 		    "mapName" string
 * [2,2]  		  "sprites"
 * [2,2,1]  	    "dirigeables"
 */
void DataTreeUtils::formatGameServerDataTree(DataTree* dataTree) {

	// Branche racine
	vector<int> rootBrancheId;


	/********************************************************************/
	/* Branche des VID (Very Important Data) de l'arbre de données 		*/
	/********************************************************************/

	// Branche principale des VID
	Branche* vidBranche = dataTree->createBranche(0, rootBrancheId, TREE_VID_BRANCHE_NAME);

	// Branche de contrôle de l'arbre de données
	dataTree->createPrivateBranche(vidBranche->getBrancheFullId(), TREE_CONTROL_BRANCHE_NAME);


	/********************************************************/
	/* Branche de la Map 									*/
	/********************************************************/

	// Branche principale de la Map
	Branche* mapBranche = dataTree->createBranche(0, rootBrancheId, "map");

	// Branche info de la Map
	Branche* infoBranche = dataTree->createBranche(0, mapBranche->getBrancheFullId(), "info");
	dataTree->createValeur(0, ANY, infoBranche->getBrancheFullId(), "mapName", JktUtils::AnyData(string("")));

	// Branche des dirigeables
	Branche* spritesBranche = dataTree->createBranche(0, mapBranche->getBrancheFullId(), "sprites");

	// Branche des dirigeables
	dataTree->createBranche(0, spritesBranche->getBrancheFullId(), "dirigeables");
}

/**
 * Formatte un arbre de données vierge en créant les branches et hiérarchies suivantes :
 * [1]			"vid"
 * [1,1]		  "tree-control"
 * [1,1,1]		    "tree-state" int
 * [1,1,1]		    "tree-update-delay" int
 */
void DataTreeUtils::formatGameClientDataTree(DistantTreeProxy* distant, DataTree* dataTree) {
	vector<string> treeControlPath;
	treeControlPath.push_back(TREE_VID_BRANCHE_NAME);
	treeControlPath.push_back(TREE_CONTROL_BRANCHE_NAME);
	PrivateBranche* treeControl = (PrivateBranche*)dataTree->getBranche(distant, treeControlPath);

	dataTree->createValeur(distant, ANY, treeControl->getBrancheFullId(), "tree-state", JktUtils::AnyData(TREE_STATE::STATE_NOT_READY));
	dataTree->createValeur(distant, ANY, treeControl->getBrancheFullId(), "tree-update-clientToServer-delay", JktUtils::AnyData(Config.Reseau.getTreeUpdateClientToServerDelay()));
	dataTree->createValeur(distant, ANY, treeControl->getBrancheFullId(), "tree-update-serverToClient-delay", JktUtils::AnyData(Config.Reseau.getTreeUpdateServerToClientDelay()));
}
