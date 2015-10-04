/*
 * DistantTreeProxy.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#include <map>
#include <string>
#include <stdexcept>
#include <iostream>

using namespace std;

#include "util/CollectionsUtils.h"
#include "data/communication/message/Changement.h"
#include "data/communication/message/ClientToServer/AddBrancheFromClientChangement.h"
#include "data/communication/message/ServerToClient/AddBrancheFromServerChangement.h"
#include "data/communication/message/ClientToServer/AddValeurFromClientChangement.h"
#include "data/communication/message/ServerToClient/AddValeurFromServerChangement.h"
#include "data/communication/message/ClientToServer/UpdateValeurFromClientChangement.h"
#include "data/communication/message/ServerToClient/UpdateValeurFromServerChangement.h"
#include "data/Valeur.h"
#include "data/ValeurInt.h"
#include "data/Branche.h"

#include "data/DistantTreeProxy.h"

DistantTreeProxy::DistantTreeProxy(Interlocutor2* interlocutor) {
	_interlocutor = interlocutor;
	_updateServerToClientTime = SDL_GetTicks();;
}

DistantTreeProxy::~DistantTreeProxy() {
}

Interlocutor2* DistantTreeProxy::getInterlocutor() const {
	return _interlocutor;
}

MarqueurDistant* DistantTreeProxy::getMarqueur(Donnee* donnee) {
	return donnee->getMarqueur(this);
}

map<Donnee*, MarqueurDistant*>& DistantTreeProxy::getMarqueurs() {
	return _marqueurs;
}

MarqueurDistant* DistantTreeProxy::addMarqueur(Donnee* donnee, int donneeTmpId) {
	MarqueurDistant* marqueur = donnee->addMarqueur(this, donneeTmpId);
	_marqueurs[donnee] = marqueur;
	return marqueur;
}

void DistantTreeProxy::collecteChangementsInClientTree(vector<Changement*>& changements) {
	map<Donnee*, MarqueurDistant*>::iterator itMa;

	for(itMa = _marqueurs.begin() ; itMa != _marqueurs.end() ; itMa++) {
		MarqueurDistant* marqueur = itMa->second;
		Donnee* donnee = marqueur->getDonnee();

		Branche* branche = dynamic_cast<Branche*>(donnee);
		Changement* changement = NULL;

		if(branche) {
			// NOUVELLE BRANCHE : branche présente sur le client dont le serveur n'a pas encore connaissance (donc avec un identifiant temporaire)
			if(branche->getBrancheId() < 0) {
				changement = new AddBrancheFromClientChangement(branche->getParentBrancheIdOrTmpId(), branche->getBrancheTmpId(), branche->getRevision(), branche->getBrancheName());
			}
		}
		else {
			Valeur* valeur = (Valeur*)(donnee);

			// NOUVELLE VALEUR : valeur présente sur le client dont le serveur n'a pas encore connaissance (donc avec un identifiant temporaire)
			if(valeur->getValeurId() < 0) {
				changement = new AddValeurFromClientChangement(valeur->getBrancheIdOrTmpId(), valeur->getValeurTmpId(), valeur->getRevision(), valeur->getValeurName(), valeur->getValeurData());
			}

			// UPDATE VALEUR : Une valeur a changé et le client n'en a pas encore connaissance
			else if(valeur->getRevision() > marqueur->getConfirmedRevision()) {
				changement = new UpdateValeurFromClientChangement(valeur->getBrancheIdOrTmpId(), valeur->getValeurId(), valeur->getRevision(), valeur->getValeurData());
			}
		}

		if(changement) {
			changement->update(marqueur);
			changements.push_back(changement);
		}
	}
}

void DistantTreeProxy::collecteChangementsInServerTree(vector<Changement*>& changements) {
	map<Donnee*, MarqueurDistant*>::iterator itMa;
	Branche* branche;
	Changement* changement;
	MarqueurDistant* marqueur;
	Donnee* donnee;

	for(itMa = _marqueurs.begin() ; itMa != _marqueurs.end() ; itMa++) {
		marqueur = itMa->second;
		donnee = marqueur->getDonnee();

		branche = dynamic_cast<Branche*>(donnee);
		changement = NULL;

		if(branche) {
			// NOUVELLE BRANCHE : branche présente sur le serveur mais dont le client n'a pas connaissance
			if(marqueur->getConfirmedRevision() == MarqueurDistant::MARQUEUR_REVISION_INIT) {
				changement = new AddBrancheFromServerChangement(branche->getParentBrancheId(), branche->getBrancheId(), branche->getDonneeType(), branche->getRevision(), branche->getBrancheName());
			}
		}
		else {
			Valeur* valeur = (Valeur*)(donnee);

			// NOUVELLE VALEUR : valeur présente sur le serveur mais dont le client n'a pas connaissance
			if(marqueur->getConfirmedRevision() == MarqueurDistant::MARQUEUR_REVISION_INIT) {
				changement = new AddValeurFromServerChangement(valeur->getBrancheId(), valeur->getValeurId(), valeur->getUpdateMode(), valeur->getRevision(), valeur->getValeurName(), valeur->getValeurData());
			}
			// VALEUR MODIFIEE : valeur présente sur le serveur dont le client a connaissance mais qui a changé
			else if(marqueur->getConfirmedRevision() != valeur->getRevision()) {
				changement = new UpdateValeurFromServerChangement(valeur->getBrancheId(), valeur->getValeurId(), valeur->getRevision(), valeur->getValeurData());
			}
		}

		if(changement) {
			changement->update(marqueur);
			changements.push_back(changement);
		}
	}
}

Uint32 DistantTreeProxy::getUpdateServerToClientTime() const {
	return _updateServerToClientTime;
}

void DistantTreeProxy::setUpdateServerToClientTime(Uint32 updateServerToClientTime) {
	_updateServerToClientTime = updateServerToClientTime;
}
