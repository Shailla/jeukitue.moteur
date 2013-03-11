/*
 * Donnee.cpp
 *
 *  Created on: 9 mars 2013
 *      Author: vgdj7997
 */

#include "data/Donnee.h"

Donnee::Donnee() {
	_revision = 0;
}

Donnee::~Donnee() {
}

void Donnee::update() {
	_revision++;

}

int Donnee::getRevision(void) const {
	return _revision;
}
