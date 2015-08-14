/*
 * LocalDataTree.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

#include "data/MarqueurDistant.h"
#include "data/exception/NotExistingBrancheException.h"
#include "data/exception/DataCommunicationException.h"
#include "data/communication/DataSerializer.h"
#include "data/communication/message/ClientToServer/AddBrancheFromClientChangement.h"
#include "data/communication/message/ClientToServer/AddValeurFromClientChangement.h"
#include "data/communication/message/ServerToClient/AcceptAddBrancheFromClientChangement.h"
#include "data/communication/message/ServerToClient/AcceptAddValeurFromClientChangement.h"
#include "data/communication/message/ClientToServer/UpdateValeurFromClientChangement.h"
#include "data/communication/message/ConfirmBrancheChangement.h"
#include "data/communication/message/ConfirmValeurChangement.h"

#include "data/LocalDataTree.h"

using namespace JktUtils;

LocalDataTree::LocalDataTree() {
}

LocalDataTree::~LocalDataTree() {
}

Branche* LocalDataTree::createBranche(const vector<int>& parentBrancheId, const string& brancheName) {
	Branche* parentBranche = getBrancheFromDistant(0, parentBrancheId);

	Branche* branche = parentBranche->createSubBrancheForServer(brancheName, DONNEE_LOCAL, 0);

	_donnees.insert(branche);

	return branche;
}

Valeur* LocalDataTree::createValeur(const vector<int>& parentBrancheId, const string& valeurName, const AnyData valeur) {
	Branche* parentBranche = getBrancheFromDistant(0, parentBrancheId);

	Valeur* val = parentBranche->createValeurForServeur(valeurName, 0, valeur);

	_donnees.insert(val);

	return val;
}

void LocalDataTree::initDistantBranche(DistantTreeProxy* distant, Branche* branche) {
	// This function must never be called
}
