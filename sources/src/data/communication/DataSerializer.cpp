/*
 * DataSerializer.cpp
 *
 *  Created on: 11 mars 2013
 *      Author: vgdj7997
 */

#include <sstream>
#include <vector>
#include <iostream>

using namespace std;

#include "data/communication/DataSerializer.h"
#include "data/communication/message/AddBrancheChangement.h"

DataSerializer::DataSerializer() {
}

DataSerializer::~DataSerializer() {
}

void DataSerializer::toStream(vector<Changement*>& changements, ostringstream& out) {
	vector<Changement*>::iterator it;

	for(it = changements.begin() ; it != changements.end() ; it++) {
		(*it)->serialize(out);
		cout << endl << "Construction message : '" << out.str();
	}
}

void DataSerializer::fromStream(vector<Changement*>& changements, istringstream& in) {
	in.exceptions(istringstream::eofbit);

	int messageType;
	Changement* changement;

	cout << endl << "Message vraiment reçu : '" << in.str() << "'";

	try {
		in >> messageType;

		switch(messageType) {
		case Changement::ADD_DATA_MESSAGE:
			changement = new AddBrancheChangement(in);
			break;

		default:
			changement = NULL;
			break;
		}

		if(changement) {
			changements.push_back(changement);
		}
	}
	catch(istringstream::failure& finDuFlux) {
		// Fin du flux, on peut ressortir
	}
}
