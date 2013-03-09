/*
 * Client.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#include <map>
#include <string>

using namespace std;

#include "data/Client.h"

Client::Client(const string& debugName) {
	_debugName = debugName;
}

Client::~Client() {
}

string Client::getDebugName() {
	return _debugName;
}

MarqueurBrancheClient* Client::getMarqueurBranche(Branche* branche) {
	_marqueursBranche.at(branche);
}

MarqueurValeurClient* Client::getMarqueurValeur(Valeur* valeur) {
	_marqueursValeur.at(valeur);
}

map<Branche*, MarqueurBrancheClient*>& Client::getMarqueursBranche() {
	return _marqueursBranche;
}

map<Valeur*, MarqueurValeurClient*>& Client::getMarqueursValeur() {
	return _marqueursValeur;
}

void Client::addMarqueur(MarqueurBrancheClient* marqueur) {
	Branche* branche = marqueur->getBranche();
	_marqueursBranche[branche] = marqueur;
}

void Client::addMarqueur(MarqueurValeurClient* marqueur) {
	Valeur* valeur = marqueur->getValeur();
	_marqueursValeur[valeur] = marqueur;
}
