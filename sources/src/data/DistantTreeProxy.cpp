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

using namespace std;

DistantTreeProxy::DistantTreeProxy(Interlocutor2* interlocutor) : _control(this) {
	_interlocutor = interlocutor;
	_updateServerToClientTime = SDL_GetTicks();;
}

DistantTreeProxy::~DistantTreeProxy() {
}

DistantTreeControl& DistantTreeProxy::getControl() {
	return _control;
}

Interlocutor2* DistantTreeProxy::getInterlocutor() const {
	return _interlocutor;
}

MarqueurDistant* DistantTreeProxy::getMarqueur(Donnee* donnee) {
	return donnee->getMarqueur(this);
}

MarqueurDistant* DistantTreeProxy::addMarqueur(Donnee* donnee, int donneeTmpId) {
	MarqueurDistant* marqueur = donnee->addMarqueur(this, donneeTmpId);
	_marqueurs[donnee] = marqueur;
	return marqueur;
}

Uint32 DistantTreeProxy::getUpdateServerToClientTime() const {
	return _updateServerToClientTime;
}

void DistantTreeProxy::setUpdateServerToClientTime(Uint32 updateServerToClientTime) {
	_updateServerToClientTime = updateServerToClientTime;
}
