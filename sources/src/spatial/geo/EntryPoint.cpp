/*
 * EntryPoint.cpp
 *
 *  Created on: 6 mai 2012
 *      Author: Erwin
 */

#include "spatial/XmlVocabulaire.h"
#include "util/V3D.h"

#include "spatial/geo/EntryPoint.h"

using namespace jkt;

namespace jkt {

EntryPoint::EntryPoint(CMap* map) : MapObject(map, MapObject::ENTRYPOINT) {
}

EntryPoint::EntryPoint(CMap* map, const float entryPosition[3]) : MapObject(map, MapObject::ENTRYPOINT), _entryPosition(entryPosition) {
}

EntryPoint::~EntryPoint() {
}

MapObject* EntryPoint::clone() {
	return new EntryPoint(*this);
}

void EntryPoint::init() {

}

CV3D EntryPoint::getEntryPosition() {
	return _entryPosition;
}

bool EntryPoint::Lit(TiXmlElement* el, CMap& map, MapLogger* mapLogger) {
	// Position
	float pos[3];
	Xml::LitPosition3fv(el, Xml::POSITION, pos);
	_entryPosition = CV3D(pos);

	return true;
}

bool EntryPoint::Save(TiXmlElement* el) {
	TiXmlElement* elEntry = new TiXmlElement(Xml::ENTRYPOINT);
	el->LinkEndChild(elEntry);

	// Position
	TiXmlElement* elPos = new TiXmlElement(Xml::POSITION);
	Xml::SaveAttribut(elPos, Xml::X, _entryPosition.X);
	Xml::SaveAttribut(elPos, Xml::Y, _entryPosition.Y);
	Xml::SaveAttribut(elPos, Xml::Z, _entryPosition.Z);
	elEntry->LinkEndChild(elPos);

	return true;
}

/**
 * Echange les coordonn�es X et Y de l'objet.
 */
void EntryPoint::echangeXY() {
	float var = _entryPosition.X;
	_entryPosition.X = _entryPosition.Y;
	_entryPosition.Y = var;
}

/**
 * Echange les coordonn�es X et Z de l'objet.
 */
void EntryPoint::echangeXZ() {
	float var = _entryPosition.X;
	_entryPosition.X = _entryPosition.Z;
	_entryPosition.Z = var;
}

/**
 * Echange les coordonn�es Y et Z de l'objet.
 */
void EntryPoint::echangeYZ() {
	float var = _entryPosition.Y;
	_entryPosition.Y = _entryPosition.Z;
	_entryPosition.Z = var;
}

/**
 * Homoth�tie pond�r�e selon X, Y et Z de l'objet.
 */
void EntryPoint::scale(float scaleX, float scaleY, float scaleZ) {
	_entryPosition.X *= scaleX;
	_entryPosition.Y *= scaleY;
	_entryPosition.Z *= scaleZ;
}

void EntryPoint::translate(float x, float y, float z) {
	_entryPosition.X += x;
	_entryPosition.Y += y;
	_entryPosition.Z += z;
}

} /* namespace jkt */
