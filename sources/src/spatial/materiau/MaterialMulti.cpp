
#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/glew.h>
#include <iostream>
#include <string>
#include <fstream>

#include "spatial/XmlVocabulaire.h"
#include "spatial/materiau/Material.h"
#include "spatial/geo/GeoMaker.h"
#include "spatial/materiau/MaterialMaker.h"
#include "spatial/IfstreamMap.h"
#include "util/Trace.h"

#include "MaterialMulti.h"

using namespace std;

namespace jkt
{

CMaterialMulti::CMaterialMulti(CMap* map) : CMaterial(map) {
	m_Type = MAT_TYPE_MULTI;
	m_TabMat = 0;			// Pas de sous-matériaux
	m_NbrTex = 0;			// Pas de sous-matériaux
}

MapObject* CMaterialMulti::clone() {
	return new CMaterial(*this);
}

void CMaterialMulti::init() {
}

int CMaterialMulti::NbrTex()
{	return m_NbrTex;	}

void CMaterialMulti::NbrTex( int nbr ) {
	m_NbrTex = nbr;
	m_TabMat = new CMaterial*[nbr];	// Création du tableau des sous-matériaux

	for( int i=0 ; i<nbr ; i++ ) {
		m_TabMat[i] = 0;
	}
}

CMaterial* CMaterialMulti::getMat(int i) {
	if(i < m_NbrTex) {
		return m_TabMat[i];
	}
	else {
		return 0;
	}
}

CMaterialMulti::~CMaterialMulti() {
	if( m_TabMat ) {
		for( int i=0 ; i<m_NbrTex ; i++ )
			if( m_TabMat[ i ] )
				delete m_TabMat[ i ];

		delete[] m_TabMat;	// Création du tableau des sous-matériaux
	}
}

void CMaterialMulti::initGL() {
	for(int i=0 ; i<m_NbrTex ; i++) {
		CMaterial* mat = m_TabMat[i];

		if(mat) {
			mat->initGL();
		}
		else {
			LOGWARN(("Materiau nul"));
		}
	}
}

void CMaterialMulti::freeGL()  {
	for(int i=0 ; i<m_NbrTex ; i++) {
		CMaterial* mat = m_TabMat[i];

		if(mat) {
			mat->freeGL();
		}
		else {
			LOGWARN(("Materiau nul"));
		}
	}
}

bool CMaterialMulti::Lit(TiXmlElement* el, CMap& map, MapLogger* mapLogger) {
	// Référence
	double ref;
	if(!el->Attribute(Xml::REF, &ref))
		throw CErreur("Fichier map corrompu : Reference materiau");

	const char* reference = el->Attribute(Xml::REF);

	if(reference) {
		_reference = reference;
	}

	// Nom
	const char* nom = el->Attribute(Xml::NOM);
	if(nom) {
		setName(nom);
	}

	// Couleurs
	Xml::LitCouleur3fv(el, Xml::AMBIANTE, m_Ambient);
	Xml::LitCouleur3fv(el, Xml::DIFFUSE, m_Diffuse);
	Xml::LitCouleur3fv(el, Xml::SPECULAR, m_Specular);

	TiXmlElement* elSma = el->FirstChildElement(Xml::SOUSMATERIAUX);

	if(!elSma) {
		mapLogger->logError("Fichier map corrompu : Sous-materiaux");
		throw CErreur("Fichier map corrompu : Sous-materiaux");
	}


	// Nombre de sous-matériaux
	double nbrSSMat;
	if(!elSma->Attribute(Xml::NBR, &nbrSSMat)) {
		mapLogger->logError("Fichier map corrompu : Nombre de sous-materiaux");
		throw CErreur("Fichier map corrompu : Nombre de sous-materiaux");
	}

	// Liste des sous-matériaux
	int i=0;
	NbrTex((int)nbrSSMat);

	for(TiXmlElement* el=elSma->FirstChildElement(); el!=0; el=el->NextSiblingElement()) {
		el->Attribute(Xml::REF);

		if(i >= NbrTex()) {
			mapLogger->logError("Fichier map corrompu : Reference sous-materiau");
			throw CErreur("Fichier map corrompu : Reference sous-materiau");
		}

		CMaterial* mat = CMaterialMaker::Lit(el, map, mapLogger);
		m_TabMat[i++] = mat;
	}

	if(i != (int)nbrSSMat) {
		mapLogger->logError("Fichier map corrompu : Nombre de sous-materiaux et compte non-cohérents");
		throw CErreur("Fichier map corrompu : Nombre de sous-materiaux et compte non-cohérents");
	}

	return true;
}

bool CMaterialMulti::Save(TiXmlElement* element) {
	// Nom, référence...
	TiXmlElement* elMat = new TiXmlElement(Xml::MATERIAUMULTI);
	elMat->SetAttribute(Xml::REF, getRef());
	elMat->SetAttribute(Xml::NOM, getName());
	element->LinkEndChild(elMat);

	// Couleurs
	CGeoMaker::SaveCouleur3fv(elMat, Xml::AMBIANTE, m_Ambient);
	CGeoMaker::SaveCouleur3fv(elMat, Xml::DIFFUSE, m_Diffuse);
	CGeoMaker::SaveCouleur3fv(elMat, Xml::SPECULAR, m_Specular);

	// Sous-matériaux
	TiXmlElement* elSma = new TiXmlElement(Xml::SOUSMATERIAUX);
	elSma->SetAttribute(Xml::NBR, NbrTex());
	for( int i=0 ; i<NbrTex() ; i++ ) {
		m_TabMat[i]->Save(elSma);
	}

	elMat->LinkEndChild(elSma);

	return true;
}

const char* CMaterialMulti::toString() {
	return CMaterial::toString();
}

}	// JktMoteur
