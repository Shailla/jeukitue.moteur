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

EntryPoint::EntryPoint() {
}

EntryPoint::EntryPoint(const float entryPosition[3])
:_entryPosition(entryPosition) {
}

EntryPoint::~EntryPoint() {
}

CV3D EntryPoint::getEntryPosition() {
	return _entryPosition;
}

bool EntryPoint::Lit(TiXmlElement* element, MapLogger* mapLogger) {
	// Position
	float pos[3];
	Xml::LitPosition3fv(element, Xml::POSITION, pos);
	_entryPosition = CV3D(pos);

	return true;
}

bool EntryPoint::Save(TiXmlElement* element) {
	TiXmlElement* elEntry = new TiXmlElement(Xml::ENTRYPOINT);
	element->LinkEndChild(elEntry);

	// Position
	TiXmlElement* elPos = new TiXmlElement(Xml::POSITION);
	Xml::SaveAttribut(elPos, Xml::X, _entryPosition.X);
	Xml::SaveAttribut(elPos, Xml::Y, _entryPosition.Y);
	Xml::SaveAttribut(elPos, Xml::Z, _entryPosition.Z);
	elEntry->LinkEndChild(elPos);

	return true;
}

/**
 * Echange les coordonnées X et Y de l'objet.
 */
void EntryPoint::EchangeXY() {
	float var = _entryPosition.X;
	_entryPosition.X = _entryPosition.Y;
	_entryPosition.Y = var;
}

/**
 * Echange les coordonnées X et Z de l'objet.
 */
void EntryPoint::EchangeXZ() {
	float var = _entryPosition.X;
	_entryPosition.X = _entryPosition.Z;
	_entryPosition.Z = var;
}

/**
 * Echange les coordonnées Y et Z de l'objet.
 */
void EntryPoint::EchangeYZ() {
	float var = _entryPosition.Y;
	_entryPosition.Y = _entryPosition.Z;
	_entryPosition.Z = var;
}

/**
 * Homothétie pondérée selon X, Y et Z de l'objet.
 */
void EntryPoint::Scale(float scaleX, float scaleY, float scaleZ) {
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
