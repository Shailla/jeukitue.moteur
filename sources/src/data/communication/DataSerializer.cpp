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
#include "data/communication/message/ServerToClient/AddBrancheFromServerChangement.h"
#include "data/communication/message/ClientToServer/AddBrancheFromClientChangement.h"
#include "data/communication/message/ServerToClient/AddValeurFromServerChangement.h"
#include "data/communication/message/ClientToServer/AddValeurFromClientChangement.h"
#include "data/communication/message/ServerToClient/UpdateValeurChangement.h"
#include "data/communication/message/ServerToClient/AcceptAddBrancheFromClientChangement.h"
#include "data/communication/message/ServerToClient/AcceptAddValeurFromClientChangement.h"

#include "data/communication/message/ConfirmBrancheChangement.h"
#include "data/communication/message/ConfirmValeurChangement.h"
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

void DataSerializer::fromStream(vector<Changement*>& changements, istringstream& in) throw(DataCommunicationException) {
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
				case Changement::ADD_BRANCHE_FROM_CLIENT_MESSAGE:
					changement = new AddBrancheFromClientChangement(in);
					break;

				case Changement::ADD_BRANCHE_FROM_SERVER_MESSAGE:
					changement = new AddBrancheFromServerChangement(in);
					break;

				case Changement::ADD_VALEUR_FROM_CLIENT_MESSAGE:
					changement = new AddValeurFromClientChangement(in);
					break;

				case Changement::ADD_VALEUR_FROM_SERVER_MESSAGE:
					changement = new AddValeurFromServerChangement(in);
					break;

				case Changement::UPDATE_VALEUR_FROM_SERVER_MESSAGE:
					changement = new UpdateValeurChangement(in);
					break;

				case Changement::ACCEPT_ADD_BRANCHE_FROM_CLIENT:
					changement = new AcceptAddBrancheFromClientChangement(in);
					break;

				case Changement::ACCEPT_ADD_VALEUR_FROM_CLIENT:
					changement = new AcceptAddValeurFromClientChangement(in);
					break;

				case Changement::CONFIRM_BRANCHE_MESSAGE:
					changement = new ConfirmBrancheChangement(in);
					break;

				case Changement::CONFIRM_VALEUR_MESSAGE:
					changement = new ConfirmValeurChangement(in);
					break;

				default:
					ostringstream msg;
					msg << " Flux indechiffrable [messageType=" << messageType << "]";
					throw DataCommunicationException(msg.str());
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
		cerr << endl << __FILE__ << ":" << __LINE__ << " Une erreur s'est produite pendant la lecture du flux ou sa fin a été atteinte de manière prématurée";
	}
}
