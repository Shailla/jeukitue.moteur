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
#include "util/MathUtils.h"
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
	if(_transformation) {
		delete[] _transformation;
	}
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
	float posAjuste[4];

	if(_transformation) {
		MathUtils::multMatrixVector4x4(_transformation, pos, posAjuste);
	}
	else {
		posAjuste[0] = pos[0];
		posAjuste[1] = pos[1];
		posAjuste[2] = pos[2];
		posAjuste[3] = pos[3];
	}

	for(MapObject* geo : _geos) {
		if(geo->checkContact(posAjuste, dist)) {
			return true;
		}
	}

	return false;
}

void GeoGroup::gereContactPlayer(float posPlayer[3], CPlayer *player, float deltaTime) {
	if(_solid) {
		float posPlayerAjuste[4];

		if(_transformation) {
			MathUtils::multMatrixVector4x4(_transformation, posPlayer, posPlayerAjuste);
		}
		else {
			posPlayerAjuste[0] = posPlayer[0];
			posPlayerAjuste[1] = posPlayer[1];
			posPlayerAjuste[2] = posPlayer[2];
			posPlayerAjuste[3] = posPlayer[3];
		}

		for(MapObject* geo : _geos) {
			geo->gereContactPlayer(posPlayerAjuste, player, deltaTime);
		}
	}
}

float GeoGroup::gereLaserPlayer(float posPlayer[3], CV3D &Dir, float dist) {
	if(_solid) {
		float posPlayerAjuste[4];

		if(_transformation) {
			MathUtils::multMatrixVector4x4(_transformation, posPlayer, posPlayerAjuste);
		}
		else {
			posPlayerAjuste[0] = posPlayer[0];
			posPlayerAjuste[1] = posPlayer[1];
			posPlayerAjuste[2] = posPlayer[2];
			posPlayerAjuste[3] = posPlayer[3];
		}

		for(MapObject* geo : _geos) {
			dist = geo->gereLaserPlayer(posPlayerAjuste, Dir, dist);
		}
	}

	return dist;

}

void GeoGroup::afficheHighlighted(float r, float v, float b) {
	for(MapObject* geo : _geos) {
		geo->afficheHighlighted(r, v, b);
	}
}

void GeoGroup::affiche() {
	if(_transformation) {
		glPushMatrix();
		glMultMatrixf((float*)_transformation);
	}

	for(MapObject* geo : _geos) {
		geo->affiche();
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
		float transformation[16];

		bool a = Xml::Lit4fv(elTransformation, Xml::T1, Xml::X, Xml::Y, Xml::Z, Xml::W, transformation + 0	);
		bool b = Xml::Lit4fv(elTransformation, Xml::T2, Xml::X, Xml::Y, Xml::Z, Xml::W, transformation + 4	);
		bool c = Xml::Lit4fv(elTransformation, Xml::T3, Xml::X, Xml::Y, Xml::Z, Xml::W, transformation + 8	);
		bool d = Xml::Lit4fv(elTransformation, Xml::T4, Xml::X, Xml::Y, Xml::Z, Xml::W, transformation + 12);

		if(a && b && c && d) {
			setTransformation(transformation);
		}
		else {
			throw CErreur("Fichier Map corrompu GeoGroup 4");
		}
	}

	TiXmlElement* elGeos = el->FirstChildElement(Xml::GEOS);

	if(elGeos) {
		for(TiXmlElement* elGeo=elGeos->FirstChildElement(); elGeo!=0; elGeo=elGeo->NextSiblingElement()) {
			MapObject* object = CGeoMaker::Lit(elGeo, map, mapLogger);

			if(object) {
				_geos.push_back(object);
			}
			else {
				mapLogger->logError("Géo corrompue ?");
			}
		}
	}

	return true;
}

void GeoGroup::setTransformation(const float transformation[16]) {
	if(!transformation) {
		if(_transformation) {
			delete[] _transformation;
			_transformation = 0;
		}

		_transformation = new float[16];

		for(int i=0 ; i<16 ; i++) {
			_transformation[i] = transformation[i];
		}
	}
}

bool GeoGroup::Save(TiXmlElement* element) throw(CErreur) {
	LOGWARN(("Use of not implemented method"));
	return true;
}

} /* namespace jkt */
