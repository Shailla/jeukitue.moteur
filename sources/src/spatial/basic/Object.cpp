/*
 * Object.cpp
 *
 *  Created on: 29 juil. 2016
 *      Author: VGDJ7997
 */

#include "spatial/basic/Object.h"

using namespace std;

namespace jkt {

CGenRef Object::GENREF;

Object::Object(CMap* map, const std::string& type) {
	_parent = map;
	_id = GENREF.genRef();	// G�n�re une r�f�rence pour l'objet
	_reference = "";
	_type = type;
	_abstract = false;
}

Object::Object(const Object& other) : Object(other._parent, other._type) {
	_id = GENREF.genRef();	// G�n�re une r�f�rence pour l'objet

	_reference = "";

	_name = other._name;
	_name += "-copy";
	_name += _id;
	_abstract = false;
}

Object::~Object() {
	GENREF.delRef(_id);		// Lib�re la r�f�rence de l'objet
}

void Object::init() throw(CErreur) {
}

bool Object::isAbstract() const {
	return _abstract;
}

void Object::setAbstract(bool abstract) {
	_abstract = abstract;
}

int Object::getId() const {
	return _id;
}

string Object::getReference() const {
	return _reference;
}

void Object::setName(const std::string& nom) {
	_name = nom;
}

const char* Object::getName() {
	return _name.c_str();
}

const std::string& Object::getType() const {
	return _type;
}

CMap* Object::getMap() {
	return _parent;
}

void Object::setMap(CMap* map) {
	_parent = map;
}

const char* Object::toString() {
	_tostring = _name;
	return _tostring.c_str();
}

} /* namespace jkt */
