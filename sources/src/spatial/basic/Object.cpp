/*
 * Object.cpp
 *
 *  Created on: 29 juil. 2016
 *      Author: VGDJ7997
 */

#include "spatial/basic/Object.h"

namespace jkt {

CGenRef Object::GENREF;

Object::Object(CMap* map) {
	_map = map;
	_reference = GENREF.genRef();	// Génère une référence pour l'objet
}

Object::Object(const Object& other) {
	_map = other._map;
	_reference = GENREF.genRef();	// Génère une référence pour l'objet
	_name = other._name;
	_name += "-copy";
	_name += _reference;
}

Object::~Object() {
	GENREF.delRef(_reference);		// Libère la référence de l'objet
}

void Object::init() throw(CErreur) {
}

unsigned int Object::getReference() const {
	return _reference;
}

void Object::setName(const std::string &nom) {
	_name = nom;
}

const char* Object::getName() {
	return _name.c_str();
}

CMap* Object::getMap() {
	return _map;
}

void Object::setMap(CMap* map) {
	_map = map;
}

const char* Object::toString() {
	_tostring = _name;
	return _tostring.c_str();
}

} /* namespace jkt */
