/*
 * DataTreeUtils.cpp
 *
 *  Created on: 19 déc. 2014
 *      Author: VGDJ7997
 */

#include "data/DataTreeUtils.h"

DataTreeUtils::DataTreeUtils() {
}

DataTreeUtils::~DataTreeUtils() {
}

/**
 * Formatte un arbre de données vierge en créant les branches et hiérarchies suivantes :
 * [1]			"vid"
 * [1,1]		  "dashboard"
 * [1,1,1]		    "state" int
 * [2]  		"map"
 * [2,1]		  "info"
 * [2,1]-1 		    "mapName" string
 * [2,2]  		  "sprites"
 * [2,2,1]  	    "dirigeables"
 */
void DataTreeUtils::formatGameDataTree(DataTree* dataTree) {

	// Branche racine
	vector<int> rootBrancheId;


	/********************************************************************/
	/* Branche des VID (Very Important Data) de l'arbre de données 		*/
	/********************************************************************/

	// Branche principale des VID
	Branche* vidBranche = dataTree->createBranche(rootBrancheId, "vid");

	// Branche de contrôle de l'arbre de données
	Branche* metadataBranche = dataTree->createBranche(vidBranche->getBrancheFullId(), "dashboard");
	dataTree->createValeur(metadataBranche->getBrancheFullId(), "state", JktUtils::AnyData(DataTree::STATE_STARTING));


	/********************************************************/
	/* Branche de la Map 									*/
	/********************************************************/

	// Branche principale de la Map
	Branche* mapBranche = dataTree->createBranche(rootBrancheId, "map");

	// Branche info de la Map
	Branche* infoBranche = dataTree->createBranche(mapBranche->getBrancheFullId(), "info");
	dataTree->createValeur(infoBranche->getBrancheFullId(), "mapName", JktUtils::AnyData(string("")));

	// Branche des dirigeables
	Branche* spritesBranche = dataTree->createBranche(mapBranche->getBrancheFullId(), "sprites");

	// Branche des dirigeables
	dataTree->createBranche(spritesBranche->getBrancheFullId(), "dirigeables");
}
