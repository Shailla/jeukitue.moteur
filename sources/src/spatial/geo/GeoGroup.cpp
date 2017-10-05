/*
 * GeoGroup.cpp
 *
 *  Created on: 4 oct. 2017
 *      Author: Erwin
 */

#include <string>
#include <sstream>

#include "util/Trace.h"
#include "util/Erreur.h"
#include "spatial/XmlVocabulaire.h"
#include "spatial/Map.h"
#include "spatial/geo/GeoMaker.h"

#include "spatial/geo/GeoGroup.h"

using namespace std;

namespace jkt {

const char* GeoGroup::identifier = "Group";

GeoGroup::GeoGroup(CMap* map) : MapObject(map, MapObject::GEO) {
	_solid = false;
	_transformation = 0;
}

GeoGroup::GeoGroup(const GeoGroup& other) : MapObject(other) {
	_solid = false;
	this->_geos = other._geos;

	if(other._transformation) {
		this->_transformation = new float[16];

		for(int i=0 ; i<16 ; i++) {
			this->_transformation[i] = other._transformation[i];
		}
	}
	else {
		this->_transformation = 0;
	}
}

GeoGroup::~GeoGroup() {
	delete[] _transformation;
}

const char* GeoGroup::toString() {
	ostringstream ttt;
	ttt << identifier << " Nom=" << getName();

	tostring = ttt.str();

	return tostring.c_str();
}

MapObject* GeoGroup::clone() {
	return new GeoGroup(*this);
}

void GeoGroup::init() throw(CErreur) {
	for(MapObject* geo : _geos) {
		geo->init();
	}
}

void GeoGroup::initGL() {
	for(MapObject* geo : _geos) {
		geo->initGL();
	}
}

void GeoGroup::freeGL() {
	for(MapObject* geo : _geos) {
		geo->freeGL();
	}
}

bool GeoGroup::checkContact( const float pos[3], float dist ) {
	for(MapObject* geo : _geos) {
		if(geo->checkContact(pos, dist)) {
			return true;
		}
	}

	return false;
}

void GeoGroup::GereContactPlayer(float positionPlayer[3], CPlayer *player) {
	if(_solid) {
		for(MapObject* geo : _geos) {
			geo->GereContactPlayer(positionPlayer, player);
		}
	}
}

float GeoGroup::GereLaserPlayer(float pos[3], CV3D &Dir, float dist) {
	if(_solid) {
		for(MapObject* geo : _geos) {
			dist = geo->GereLaserPlayer(pos, Dir, dist);
		}
	}

	return dist;

}

void GeoGroup::AfficheHighlighted(float r, float v, float b) {
	for(MapObject* geo : _geos) {
		geo->AfficheHighlighted(r, v, b);
	}
}

void GeoGroup::Affiche() {
	if(_transformation) {
		glPushMatrix();
		glMultMatrixf((float*)_transformation);
	}

	for(MapObject* geo : _geos) {
		geo->Affiche();
	}

	if(_transformation) {
		glPopMatrix();
	}
}

bool GeoGroup::Lit(TiXmlElement* el, CMap& map, MapLogger* mapLogger) throw(CErreur) {
	// Type
	const char* type = el->Value();

	if(!type)
		throw CErreur("Fichier Map corrompu GeoGroup 1");

	if(strcmp(type, Xml::GEOGROUP))
		throw CErreur("Fichier Map corrompu GeoGroup 2");

	// Nom
	const char* nom = el->Attribute(Xml::NOM);
	if(!nom)
		throw CErreur("Fichier Map corrompu GeoGroup 3");
	setName(nom);

	// Référence
	const char* reference = el->Attribute(Xml::REF);

	if(reference) {
		_reference = reference;
	}

	// Solidité
	_solid = Xml::LitSolidite(el);

	// Transformation
	TiXmlElement* elTransformation = el->FirstChildElement(Xml::TRANSFORMATION);

	if(elTransformation) {
		_transformation = new float[16];

		bool a = Xml::Lit4fv(elTransformation, Xml::T1, Xml::X, Xml::Y, Xml::Z, Xml::W, _transformation + 0	);
		bool b = Xml::Lit4fv(elTransformation, Xml::T2, Xml::X, Xml::Y, Xml::Z, Xml::W, _transformation + 4	);
		bool c = Xml::Lit4fv(elTransformation, Xml::T3, Xml::X, Xml::Y, Xml::Z, Xml::W, _transformation + 8	);
		bool d = Xml::Lit4fv(elTransformation, Xml::T4, Xml::X, Xml::Y, Xml::Z, Xml::W, _transformation + 12);

		if( !(a && b && c && d) ) {
			throw CErreur("Fichier Map corrompu GeoGroup 4");
		}
	}

	TiXmlElement* elGeos = el->FirstChildElement(Xml::GEOS);

	if(elGeos) {
		for(TiXmlElement* elGeo=elGeos->FirstChildElement(); elGeo!=0; elGeo=elGeo->NextSiblingElement()) {
			MapObject* object = CGeoMaker::Lit(elGeo, map, mapLogger);

			if(object) {
				_geos.push_back(object);
				//map.add(object);
			}
			else {
				mapLogger->logError("Géo corrompue ?");
			}
		}
	}

	return true;
}

bool GeoGroup::Save(TiXmlElement* element) throw(CErreur) {
	LOGWARN(("Use of not implemented method"));
	return true;
}

} /* namespace jkt */
