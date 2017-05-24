
#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/glew.h>
#include <string>
#include <fstream>

#include "spatial/XmlVocabulaire.h"
#include "MaterialMaker.h"
#include "spatial/IfstreamMap.h"
#include "spatial/geo/GeoMaker.h"

#include "spatial/materiau/Material.h"

using namespace std;

namespace jkt
{

CMaterial::CMaterial(CMap* map) : MapObject(map, MapObject::MATERIAL) {
	m_Type = MAT_TYPE_SIMPLE;
	m_Ref = 0;

	m_Diffuse[0] = 0.0f;	m_Diffuse[1] = 0.0f;	m_Diffuse[2] = 0.0f;	m_Diffuse[3] = 1.0f;
	m_Ambient[0] = 0.0f;	m_Ambient[1] = 0.0f;	m_Ambient[2] = 0.0f;	m_Ambient[3] = 1.0f;
	m_Specular[0] = 0.0f;	m_Specular[1] = 0.0f;	m_Specular[2] = 0.0f;	m_Specular[3] = 1.0f;
}

CMaterial::~CMaterial() {
}

MapObject* CMaterial::clone() {
	return new CMaterial(*this);
}

void CMaterial::init() throw(CErreur) {
}

CMaterial::MAT_TYPE CMaterial::Type() const {
	return m_Type;
}

const char* CMaterial::toString() {
	switch(m_Type) {
	case CMaterial::MAT_TYPE_MULTI:
		tostring = "multi";
		break;
	case CMaterial::MAT_TYPE_SIMPLE:
		tostring = "simple";
		break;
	case CMaterial::MAT_TYPE_TEXTURE:
		tostring = "texture";
		break;
	default:
		tostring = "???";
		break;
	}

	return tostring.c_str();
}

bool CMaterial::LitFichier(CMap* map, CIfstreamMap &fichier) {
	string mot;

	fichier >> mot;
	if( mot!="Reference" )
		return false;	// Fichier corrompu

	fichier >> m_Ref;			// Référence du matériau

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

bool CMaterial::Lit(TiXmlElement* el, CMap& map, MapLogger* mapLogger) {
	double ref;

	// Nom
	const char* nom = el->Attribute(Xml::NOM);
	if(nom) {
		setName(nom);
	}

	// Référence
	el->Attribute(Xml::REF, &ref);
	m_Ref = (unsigned int)ref;

	// Couleurs
	Xml::LitCouleur3fv(el, Xml::AMBIANTE, m_Ambient);
	Xml::LitCouleur3fv(el, Xml::DIFFUSE, m_Diffuse);
	Xml::LitCouleur3fv(el, Xml::SPECULAR, m_Specular);

	return true;
}

bool CMaterial::SaveFichierMap( ofstream &fichier ) {
	fichier << "\n\nMateriauSimple";
	fichier << "\n\tReference\t" << m_Ref;
	fichier << "\n\tAmbient\t\t" << m_Ambient[0] << "\t" << m_Ambient[1] << "\t" << m_Ambient[2];
	fichier << "\n\tDiffuse\t\t" << m_Diffuse[0] << "\t" << m_Diffuse[1] << "\t" << m_Diffuse[2];
	fichier << "\n\tSpecular\t" << m_Specular[0] << "\t" << m_Specular[1] << "\t" << m_Specular[2];

	return true;
}

bool CMaterial::Save(TiXmlElement* element) {
	// Nom, référence...
	TiXmlElement* elMat = new TiXmlElement(Xml::MATERIAU);
	elMat->SetAttribute(Xml::TYPE,Xml::SIMPLE);
	elMat->SetAttribute(Xml::REF,m_Ref);
	elMat->SetAttribute(Xml::NOM, getName());
	element->LinkEndChild(elMat);

	// Couleurs
	CGeoMaker::SaveCouleur3fv(elMat, Xml::AMBIANTE, m_Ambient);
	CGeoMaker::SaveCouleur3fv(elMat, Xml::DIFFUSE, m_Diffuse);
	CGeoMaker::SaveCouleur3fv(elMat, Xml::SPECULAR, m_Specular);

	return true;
}

void CMaterial::Active() {
	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, m_Ambient );
	glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, m_Diffuse );
	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, m_Specular );
	glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, 50.0f );
}

void CMaterial::Desactive() {
}

unsigned int CMaterial::getRef() const {	// Renvoie la référence du matériau
	return m_Ref;
}

void CMaterial::setRef(unsigned int ref) {
	m_Ref = ref;
}

}	// JktMoteur
