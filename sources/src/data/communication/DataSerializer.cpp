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
#include "data/communication/message/AddValeurChangement.h"
#include "data/communication/message/UpdateValeurChangement.h"
#include "util/StreamUtils.h"

DataSerializer::DataSerializer() {
}

DataSerializer::~DataSerializer() {
}

void DataSerializer::toStream(vector<Changement*>& changements, ostringstream& out) {
	vector<Changement*>::iterator it;

	for(it = changements.begin() ; it != changements.end() ; it++) {
		(*it)->serialize(out);
	}
}

void DataSerializer::fromStream(vector<Changement*>& changements, istringstream& in) {
	in.exceptions(istringstream::eofbit);

	int messageType;
	Changement* changement;

	try {
		for(;;) {
			try {
				StreamUtils::read(in, messageType);
			}
			catch(istringstream::failure& finDuFlux) {
				throw StreamUtils::EndOfStreamException();
			}

			try {
				switch(messageType) {
				case Changement::ADD_BRANCHE_MESSAGE:
					changement = new AddBrancheChangement(in);
					break;

				case Changement::ADD_VALEUR_MESSAGE:
					changement = new AddValeurChangement(in);
					break;

				case Changement::UPDATE_VALEUR_MESSAGE:
					changement = new UpdateValeurChangement(in);
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
				throw StreamUtils::ReadStreamError();
			}
		}
	}
	catch(StreamUtils::EndOfStreamException& exception) {
		// La fin du flux a été atteinte, on peut ressortir
	}
	catch(StreamUtils::ReadStreamError& error) {
		cerr << endl << "Une erreur s'est produite pendant la lecture du flux ou sa fin a été atteinte de manière prématurée";
	}
}
