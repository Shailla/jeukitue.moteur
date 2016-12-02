
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

#include "SDL.h"

#include "spatial/XmlVocabulaire.h"
#include "spatial/IfstreamMap.h"
#include "spatial/geo/GeoMaker.h"
#include "spatial/materiau/Textures.h"
#include "spatial/widget/Texture.h"
#include "util/Trace.h"
#include "spatial/materiau/Material.h"
#include "spatial/materiau/MaterialMaker.h"
#include "ressource/RessourcesLoader.h"

#include "spatial/materiau/MaterialTexture.h"

namespace jkt
{

CMaterialTexture::CMaterialTexture()
:CMaterial()
{
	m_Type = MAT_TYPE_TEXTURE;
	_texture = NULL;
}

CMaterialTexture::CMaterialTexture( const CMaterial &mat ) {
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

CMaterialTexture::~CMaterialTexture()
{
}

void CMaterialTexture::initGL() throw(jkt::CErreur) {
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

bool CMaterialTexture::Lit(TiXmlElement* element, string &repertoire, MapLogger* mapLogger) {
	double ref;

	// Référence
	if(!element->Attribute(Xml::REF, &ref)) {
		mapLogger->logError("Fichier map corrompu : Reference materiau");
		throw CErreur("Fichier map corrompu : Reference materiau");
	}

	m_Ref = (unsigned int)ref;

	// Couleurs
	Xml::LitCouleur3fv(element, Xml::AMBIANTE, m_Ambient);
	Xml::LitCouleur3fv(element, Xml::DIFFUSE, m_Diffuse);
	Xml::LitCouleur3fv(element, Xml::SPECULAR, m_Specular);

	// Fichier de texture
	TiXmlElement* elFic = element->FirstChildElement(Xml::FICHIER);

	if(!elFic) {
		mapLogger->logError("Fichier map corrompu : Nom de fichier de texture");
		throw CErreur("Fichier map corrompu : Nom de fichier de texture");
	}

	m_FichierTexture = elFic->Attribute(Xml::NOM);
	RessourcesLoader::getFileRessource(repertoire, m_FichierTexture);

	return true;
}

bool CMaterialTexture::LitFichier( CIfstreamMap &fichier ) {
	string mot;
	fichier >> mot;
	if( mot!="Reference" )
		return false;

	fichier >> m_Ref;			// Référence du matériau

	fichier >> mot;				// "Fichier de texture"
	if( mot!="FichierDeTexture" )
		return false;	// Fichier corrompu

	fichier >> m_FichierTexture;
	RessourcesLoader::getFileRessource(fichier.getFileName(),m_FichierTexture);

	fichier >> mot;
	if( mot!="Ambient" )
		return false;	// Fichier corrompu

	fichier >> m_Ambient[0] >> m_Ambient[1] >> m_Ambient[2];

	fichier >> mot;
	if( mot!="Diffuse" )
		return false;	// Fichier corrompu

	fichier >> m_Diffuse[0] >> m_Diffuse[1] >> m_Diffuse[2];

	fichier >> mot;
	if( mot!="Specular" )
		return false;

	fichier >> m_Specular[0] >> m_Specular[1] >> m_Specular[2];

	return true;
}

bool CMaterialTexture::SaveFichierMap( ofstream &fichier ) {
	// Le fichier de texture a été transféré dans le répertoire de la Map
	// Ce répertoire est représenté par le caractère '$' dans une Map
	// Trouve le dernier séparateur ('/' ou '\') et remplace ce qui précède par '$'
	// Exemple : /Chateau/Brique.jpg devient $Brique.jpg
	string fichierTexture = m_FichierTexture;

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

	// Enregistre dans le fichier
	fichier << "\n\tFichierDeTexture\t" << fichierTexture;
	fichier << "\n\tAmbient\t\t" << m_Ambient[0] << "\t" << m_Ambient[1] << "\t" << m_Ambient[2];
	fichier << "\n\tDiffuse\t\t" << m_Diffuse[0] << "\t" << m_Diffuse[1] << "\t" << m_Diffuse[2];
	fichier << "\n\tSpecular\t" << m_Specular[0] << "\t" << m_Specular[1] << "\t" << m_Specular[2];

	return true;
}

bool CMaterialTexture::Save(TiXmlElement* element) {
	// Nom, référence...
	TiXmlElement* elMat = new TiXmlElement(Xml::MATERIAU);
	elMat->SetAttribute(Xml::TYPE, Xml::TEXTURE);
	elMat->SetAttribute(Xml::REF, getRef());
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
