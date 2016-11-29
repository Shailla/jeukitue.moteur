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
	_id = GENREF.genRef();	// Génère une référence pour l'objet
	_reference = -1;
}

Object::Object(const Object& other) : Object(other._map) {
	_id = GENREF.genRef();	// Génère une référence pour l'objet
	_reference = -1;
	_name = other._name;
	_name += "-copy";
	_name += _id;
}

Object::~Object() {
	GENREF.delRef(_id);		// Libère la référence de l'objet
}

void Object::init() throw(CErreur) {
}

unsigned int Object::getId() const {
	return _id;
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
