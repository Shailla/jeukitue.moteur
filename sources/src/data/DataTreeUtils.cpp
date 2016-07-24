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


	/********************************************************/
	/* Branche de la Map 									*/
	/********************************************************/

	// Branche principale de la Map
	Branche* mapBranche = dataTree->createBranche(0, rootBrancheId, "map");

	// Branche info de la Map
	Branche* infoBranche = dataTree->createBranche(0, mapBranche->getBrancheFullId(), "info");
	dataTree->createValeur(0, ANY, infoBranche->getBrancheFullId(), "mapName", jkt::AnyData(string("")));

	// Branche des dirigeables
	Branche* spritesBranche = dataTree->createBranche(0, mapBranche->getBrancheFullId(), "sprites");

	// Branche des dirigeables
	dataTree->createBranche(0, spritesBranche->getBrancheFullId(), "dirigeables");
}
