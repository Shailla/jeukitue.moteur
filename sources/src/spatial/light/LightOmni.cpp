
#include <fstream>

#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/glu.h>

#include "spatial/XmlVocabulaire.h"
#include "spatial/IfstreamMap.h"
#include "spatial/geo/GeoMaker.h"
#include "spatial/light/Light.h"

#include "spatial/light/LightOmni.h"

using namespace std;

namespace jkt
{

CLightOmni::CLightOmni(CMap* map) : CLight(map) {
}

CLightOmni::~CLightOmni() {
}

MapObject* CLightOmni::clone() {
	return new CLightOmni(*this);
}

void CLightOmni::init() {

}

bool CLightOmni::Save(TiXmlElement* element) {
	// Nom, r�f�rence, type
	TiXmlElement* elLum = new TiXmlElement(Xml::LIGHTOMNI);
	elLum->SetAttribute(Xml::REF, m_refLight);
	element->LinkEndChild(elLum);

	// Couleurs
	CGeoMaker::SaveCouleur3fv(elLum, Xml::AMBIANTE, m_ColorAmbient);
	CGeoMaker::SaveCouleur3fv(elLum, Xml::DIFFUSE, m_ColorDiffuse);
	CGeoMaker::SaveCouleur3fv(elLum, Xml::SPECULAR, m_ColorSpecular);

	// Position
	TiXmlElement* elPos = new TiXmlElement(Xml::POSITION);
	Xml::SaveAttribut(elPos, Xml::X, m_Position[0]);
	Xml::SaveAttribut(elPos, Xml::Y, m_Position[1]);
	Xml::SaveAttribut(elPos, Xml::Z, m_Position[2]);
	elLum->LinkEndChild(elPos);

	return true;
}

void CLightOmni::Active() {
	float fff[4];
	fff[0] = m_Position[0];
	fff[1] = m_Position[1];
	fff[2] = m_Position[2];
	fff[3] = 1.0f;

	glPushMatrix();

	glTranslatef( fff[0], fff[1], fff[2] );

	glDisable( GL_LIGHTING );
	glColor3f( 0.0f, 1.0f, 1.0f );

	GLUquadric* quadric = gluNewQuadric();
	gluSphere(quadric, 0.05, 64, 64);
	gluDeleteQuadric(quadric);

	glEnable( GL_LIGHTING );

	glPopMatrix();

	CLight::Active();

	glLightfv( m_refLight, GL_POSITION, fff );
	glLightfv( m_refLight, GL_AMBIENT, m_ColorAmbient );
	glLightfv( m_refLight, GL_DIFFUSE, m_ColorDiffuse );
	glLightfv( m_refLight, GL_SPECULAR, m_ColorSpecular );
}

bool CLightOmni::Lit(TiXmlElement* el, CMap& map, MapLogger* mapLogger) {
	// Type
	const char* type = el->Value();
	if(!type) {
		mapLogger->logError("Fichier Map corrompu : Type lumiere manquant");
		throw CErreur("Fichier Map corrompu : Type lumiere manquant");
	}

	if(strcmp(Xml::LIGHTOMNI, type)) {
		mapLogger->logError("Fichier Map corrompu : Type incompatible");
		throw CErreur("Fichier Map corrompu : Type incompatible");
	}

	// Référence
	const char* reference = el->Attribute(Xml::REF);

	if(reference) {
		_reference = reference;
	}

	// Couleurs
	Xml::LitCouleur3fv(el, Xml::AMBIANTE, m_ColorAmbient);
	Xml::LitCouleur3fv(el, Xml::DIFFUSE, m_ColorDiffuse);
	Xml::LitCouleur3fv(el, Xml::SPECULAR, m_ColorSpecular);

	// Position
	Xml::LitPosition3fv(el, Xml::POSITION, m_Position);

	return true;
}

void CLightOmni::Desactive()
{
	CLight::Desactive();
}

}	// JktMoteur
