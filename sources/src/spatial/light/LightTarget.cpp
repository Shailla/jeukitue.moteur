
#include <fstream>
#include <iostream>

#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/glu.h>

#include "spatial/XmlVocabulaire.h"
#include "spatial/materiau/MaterialMaker.h"
#include "util/math_vectoriel.h"
#include "spatial/IfstreamMap.h"
#include "spatial/geo/GeoMaker.h"
#include "spatial/light/Light.h"

#include "spatial/light/LightTarget.h"

using namespace std;

namespace jkt
{

CLightTarget::CLightTarget(CMap* map) : CLight(map) {
	m_fFallOff = 0.0f;
}

CLightTarget::~CLightTarget(void) {
}

MapObject* CLightTarget::clone() {
	return new CLightTarget(*this);
}

void CLightTarget::init() {

}

void CLightTarget::getDirection(float direction[3]) const {
	direction[ 0 ] = m_Direction[ 0 ];
	direction[ 1 ] = m_Direction[ 1 ];
	direction[ 2 ] = m_Direction[ 2 ];
}

float CLightTarget::getFallOff() const {
	return m_fFallOff;
}

void CLightTarget::SetDirection( float direction[3] ) {
	m_Direction[ 0 ] = direction[ 0 ];
	m_Direction[ 1 ] = direction[ 1 ];
	m_Direction[ 2 ] = direction[ 2 ];
	m_Direction[ 3 ] = 1.0f;
}

bool CLightTarget::Lit(TiXmlElement* el, CMap& map, MapLogger* mapLogger) {
	// Type
	const char* type = el->Value();

	if(!type) {
		mapLogger->logError("Fichier Map corrompu : Type lumiere manquant");
		throw CErreur("Fichier Map corrompu : Type lumiere manquant");
	}

	if(strcmp(Xml::LIGHTTARGET, type)) {
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

	// Direction
	Xml::LitDirection3fv(el, Xml::DIRECTION, m_Direction);

	// Fall off
	m_fFallOff = (float)Xml::LitValeur(el, Xml::FALLOFF);

	return true;
}

bool CLightTarget::Save(TiXmlElement* element) {
	// Nom, r�f�rence, type
	TiXmlElement* elLum = new TiXmlElement(Xml::LIGHTTARGET);
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

	// Direction
	TiXmlElement* elDir = new TiXmlElement(Xml::DIRECTION);
	Xml::SaveAttribut(elDir, Xml::X, m_Direction[0]);
	Xml::SaveAttribut(elDir, Xml::Y, m_Direction[1]);
	Xml::SaveAttribut(elDir, Xml::Z, m_Direction[2]);
	elLum->LinkEndChild(elDir);

	// Fall off
	TiXmlElement* elFall = new TiXmlElement(Xml::FALLOFF);
	Xml::SaveAttribut(elFall, Xml::VALEUR, m_fFallOff);
	elLum->LinkEndChild(elFall);

	return true;
}

void CLightTarget::Active()
{
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

	glColor3f( 0.0f, 1.0f, 0.0f );
	glBegin( GL_LINES );
		glVertex3f( 0.0f, 0.0f, 0.0f );
		glVertex3fv( m_Direction );
	glEnd();

	glPopMatrix();

	CLight::Active();

	glLightfv( m_refLight, GL_POSITION, fff );
	glLightfv( m_refLight, GL_AMBIENT, m_ColorAmbient );
	glLightfv( m_refLight, GL_DIFFUSE, m_ColorDiffuse );
	glLightfv( m_refLight, GL_SPECULAR, m_ColorSpecular );
	glLightfv( m_refLight, GL_SPOT_DIRECTION, m_Direction );
	glLightf( m_refLight, GL_SPOT_CUTOFF, m_fFallOff );
}

void CLightTarget::Desactive()
{
	CLight::Desactive();
}

void CLightTarget::echangeXY()		// Echange les coordonn�es X et Y de l'objet
{
	CLight::echangeXY();

	float var = m_Direction[0];
	m_Direction[0] = m_Direction[1];
	m_Direction[1] = var;
}
void CLightTarget::echangeXZ()			// Echange les coordonn�es X et Z de l'objet
{
	CLight::echangeXZ();

	float var = m_Direction[0];
	m_Direction[0] = m_Direction[2];
	m_Direction[2] = var;
}

void CLightTarget::echangeYZ()			// Echange les coordonn�es Y et Z de l'objet
{
	CLight::echangeYZ();

	float var = m_Direction[1];
	m_Direction[1] = m_Direction[2];
	m_Direction[2] = var;
}

void CLightTarget::scale( float scaleX, float scaleY, float scaleZ )	// Homoth�tie pond�r�e selon X, Y et Z de l'objet
{
	CLight::scale( scaleX, scaleY, scaleZ );

	m_Direction[ 0 ] *= scaleX;
	m_Direction[ 1 ] *= scaleY;
	m_Direction[ 2 ] *= scaleZ;

	normalise( m_Direction );
}

void CLightTarget::SetFallOff( float falloff )
{
	m_fFallOff = falloff;
}

}	// JktMoteur
