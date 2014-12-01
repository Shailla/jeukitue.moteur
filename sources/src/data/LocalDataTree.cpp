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

#include "util/StringUtils.h"
#include "util/types/IntData.h"
#include "data/ValeurInt.h"
#include "data/MarqueurDistant.h"
#include "data/exception/NotExistingBrancheException.h"
#include "data/exception/DataCommunicationException.h"
#include "data/communication/DataSerializer.h"
#include "data/communication/message/ServerToClient/AddBrancheFromServerChangement.h"
#include "data/communication/message/ServerToClient/AddValeurFromServerChangement.h"
#include "data/communication/message/ServerToClient/AcceptAddBrancheFromClientChangement.h"
#include "data/communication/message/ServerToClient/AcceptAddValeurFromClientChangement.h"
#include "data/communication/message/ServerToClient/UpdateValeurFromServerChangement.h"
#include "data/communication/message/ConfirmBrancheChangement.h"
#include "data/communication/message/ConfirmValeurChangement.h"
#include "util/CollectionsUtils.h"
#include "data/DistantTreeProxy.h"

#include "data/LocalDataTree.h"

using namespace JktUtils;

LocalDataTree::LocalDataTree() {
}

LocalDataTree::~LocalDataTree() {
}

void LocalDataTree::initDistantBranche(DistantTreeProxy* distant, Branche* branche) {
}

Branche* LocalDataTree::createBranche(const std::vector<int>& parentBrancheId, const std::string& brancheName) {
	Branche* parentBranche = getBranche(parentBrancheId);
	Branche* branche = parentBranche->createSubBrancheForClient(brancheName, 0);

	return branche;
}

Valeur* LocalDataTree::createValeur(const std::vector<int>& parentBrancheId, const std::string& valeurName, const Data* value) {
	Branche* parentBranche = getBranche(parentBrancheId);
	Valeur* valeur = parentBranche->createValeurForClient(valeurName, 0, value);

	return valeur;
}

Branche* LocalDataTree::getBrancheByTmpId(const vector<int>& parentBrancheId, int brancheTmpId) throw(NotExistingBrancheException) {
	vector<int>::const_iterator iter;

	Branche* parentBranche = &getRoot();

	for(iter = parentBrancheId.begin() ; (iter != parentBrancheId.end() && parentBranche != NULL) ; iter++) {
		parentBranche = parentBranche->getSubBrancheByIdOrTmpId(*iter);
	}

	if(!parentBranche) {
		throw NotExistingBrancheException();
	}

	Branche* branche = parentBranche->getSubBrancheByIdOrTmpId(brancheTmpId);

	if(!branche) {
		throw NotExistingBrancheException();
	}

	return branche;
}
