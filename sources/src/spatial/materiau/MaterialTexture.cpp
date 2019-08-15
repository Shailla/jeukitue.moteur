
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <iostream>

#include "SDL.h"

#include "spatial/XmlVocabulaire.h"
#include "spatial/IfstreamMap.h"
#include "spatial/geo/GeoMaker.h"
#include "spatial/materiau/Textures.h"
#include "spatial/widget/Texture.h"
#include "util/Trace.h"
#include "spatial/Map.h"
#include "spatial/materiau/Material.h"
#include "spatial/materiau/MaterialMaker.h"
#include "ressource/RessourcesLoader.h"

#include "spatial/materiau/MaterialTexture.h"

using namespace std;

namespace jkt
{

CMaterialTexture::CMaterialTexture(CMap* map) : CMaterial(map) {
	m_Type = MAT_TYPE_TEXTURE;
	_texture = NULL;
}

CMaterialTexture::CMaterialTexture(CMap* map, const CMaterial &mat) : CMaterial(map) {
	// Copie des attributs de 'mat'
	m_Type = MAT_TYPE_TEXTURE;

	m_Ambient[0] = mat.m_Ambient[0];	m_Ambient[1] = mat.m_Ambient[1];		m_Ambient[2] = mat.m_Ambient[2];
	m_Diffuse[0] = mat.m_Diffuse[0];	m_Diffuse[1] = mat.m_Diffuse[1];	m_Diffuse[2] = mat.m_Diffuse[2];
	m_Specular[0] = mat.m_Specular[0];	m_Specular[1] = mat.m_Specular[1];	m_Specular[2] = mat.m_Specular[2];

	m_Ref = mat.getRef();

	// Construction des attributs de l'instance
	m_Type = MAT_TYPE_TEXTURE;
	_texture = NULL;
}

CMaterialTexture::~CMaterialTexture() {
}

MapObject* CMaterialTexture::clone() {
	return new CMaterial(*this);
}

void CMaterialTexture::init() {
}

void CMaterialTexture::initGL() {
	try {
		RessourcesLoader::getFileRessource(m_FichierTexture);
		Uint8* pixels = litFichierImage(m_FichierTexture, 1.0f);
		_texture = litFichierTexture(m_FichierTexture, 1.0f, pixels);
	}
	catch(CErreur& erreur) {
		LOGWARN(("Echec de lecture de la texture '%s'", m_FichierTexture.c_str()));
	}
}

void CMaterialTexture::freeGL() {
	// TODO
}

bool CMaterialTexture::Lit(TiXmlElement* el, CMap& map, MapLogger* mapLogger) {
	// Référence
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

	// Fichier de texture
	TiXmlElement* elFic = el->FirstChildElement(Xml::FICHIER);

	if(!elFic) {
		mapLogger->logError("Fichier map corrompu : Nom de fichier de texture");
		throw CErreur("Fichier map corrompu : Nom de fichier de texture");
	}

	m_FichierTexture = elFic->Attribute(Xml::NOM);
	RessourcesLoader::getFileRessource(map.getBinariesDirectory(), m_FichierTexture);

	return true;
}

bool CMaterialTexture::Save(TiXmlElement* element) {
	// Nom, référence...
	TiXmlElement* elMat = new TiXmlElement(Xml::MATERIAUTEXTURE);
	elMat->SetAttribute(Xml::REF, getRef());
	elMat->SetAttribute(Xml::NOM, getName());
	element->LinkEndChild(elMat);

	// Couleurs de matériau
	CGeoMaker::SaveCouleur3fv(elMat, Xml::AMBIANTE, m_Ambient);
	CGeoMaker::SaveCouleur3fv(elMat, Xml::DIFFUSE, m_Diffuse);
	CGeoMaker::SaveCouleur3fv(elMat, Xml::SPECULAR, m_Specular);

	// Fichier de texture
	string fichierTexture = m_FichierTexture;

	// Le fichier de texture a été transféré dans le répertoire de la Map
	// Ce répertoire est représenté par le caractère '$' dans une Map
	// Trouve le dernier séparateur ('/' ou '\') et remplace ce qui précède par '$'
	// Exemple : /Chateau/Brique.jpg devient $Brique.jpg
	size_t backSlashPos = (int)fichierTexture.find_last_of('\\');
	size_t slashPos = (int)fichierTexture.find_last_of('/');

	size_t dollarPos;

	if(slashPos < fichierTexture.size() && backSlashPos < fichierTexture.size()) {
		dollarPos = (slashPos > backSlashPos)? slashPos : backSlashPos;
	}
	else if(slashPos < fichierTexture.size()) {
		dollarPos = slashPos;
	}
	else if(backSlashPos < fichierTexture.size()) {
		dollarPos = backSlashPos;
	}
	else {
		dollarPos = 0;
	}

	fichierTexture.replace(0, dollarPos, "$");

	// Enregistre dans le fichier XML
	TiXmlElement* elFic = new TiXmlElement(Xml::FICHIER);
	elFic->SetAttribute(Xml::NOM, fichierTexture.c_str());
	elMat->LinkEndChild(elFic);

	return true;
}

void CMaterialTexture::Active() {
	if(_texture) {
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glBindTexture(GL_TEXTURE_2D, _texture->getGlTexName());
	}
}

void CMaterialTexture::Desactive() {
	glDisable( GL_TEXTURE_2D );
}

}	// JktMoteur
