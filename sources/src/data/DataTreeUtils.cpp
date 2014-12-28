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
 * [1]  			"map"
 * [1,1]			  "info"
 * [1,1]-1 			    "mapName" string
 * [1,2]  			  "sprites"
 * [1,2,1]  		    "dirigeables"
 */
void DataTreeUtils::formatGameDataTree(DataTree* dataTree) {

	/****************************/
	/* Branche de la Map 		*/
	/****************************/

	// Branche principale de la Map
	vector<int> mapBrancheId;
	Branche* mapBranche = dataTree->createBranche(mapBrancheId, "map");

	// Branche info de la Map
	Branche* infoBranche = dataTree->createBranche(mapBranche->getBrancheFullId(), "info");
	dataTree->createValeur(infoBranche->getBrancheFullId(), "mapName", JktUtils::AnyData(string("")));

	// Branche des dirigeables
	Branche* spritesBranche = dataTree->createBranche(mapBranche->getBrancheFullId(), "sprites");

	// Branche des dirigeables
	dataTree->createBranche(spritesBranche->getBrancheFullId(), "dirigeables");
}
