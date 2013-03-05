/*
 * Client.cpp
 *
 *  Created on: 5 mars 2013
 *      Author: vgdj7997
 */

#include "data/Client.h"

Client::Client() {
}

Client::~Client() {
}

vector<MarqueurClient>& Client::getMarqueurs() {
	return _marqueurs;
}

void Client::addMarqueur(MarqueurClient& marqueur) {
	_marqueurs.push_back(marqueur);
}
