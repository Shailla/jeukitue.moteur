/*
 * DataSerializer.cpp
 *
 *  Created on: 11 mars 2013
 *      Author: vgdj7997
 */

#include <sstream>
#include <vector>

using namespace std;

#include "data/communication/DataSerializer.h"

DataSerializer::DataSerializer() {
}

DataSerializer::~DataSerializer() {
}

char* DataSerializer::toBytes(vector<Changement*>& changements) {
	ostringstream out;

	vector<Changement*>::iterator it;

	for(it = changements.begin() ; it != changements.end() ; it++) {
		Changement* chgt = *it;
	}

	return NULL;
}
