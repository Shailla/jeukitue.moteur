/*
 * Donnee.cpp
 *
 *  Created on: 9 mars 2013
 *      Author: vgdj7997
 */

#include "data/Donnee.h"

Donnee::Donnee() {
	revision = 0;
}

Donnee::~Donnee() {
}

void Donnee::update() {
	revision++;

}
