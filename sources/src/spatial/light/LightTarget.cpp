
#include <fstream>
#include <iostream>

using namespace std;

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

namespace jkt
{

CLightTarget::CLightTarget(CMap* map) : CLight(map) {
	m_fFallOff = 0.0f;
}

CLightTarget::~CLightTarget(void) {
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

bool CLightTarget::Lit(TiXmlElement* element, MapLogger* mapLogger) {
	double ref;
	const char* type;

	// R�f�rence
	if(!element->Attribute(Xml::REF, &ref)) {
		mapLogger->logError("Fichier Map corrompu : Lumiere ref");
		throw CErreur("Fichier Map corrompu : Lumiere ref");
	}
	m_refLight = (int)ref;

	// Type
	type = element->Attribute(Xml::TYPE);
	if(!type) {
		mapLogger->logError("Fichier Map corrompu : Type lumiere manquant");
		throw CErreur("Fichier Map corrompu : Type lumiere manquant");
	}

	if(strcmp(Xml::TARGET, type)) {
		mapLogger->logError("Fichier Map corrompu : Type incompatible");
		throw CErreur("Fichier Map corrompu : Type incompatible");
	}

	// Couleurs
	Xml::LitCouleur3fv(element, Xml::AMBIANTE, m_ColorAmbient);
	Xml::LitCouleur3fv(element, Xml::DIFFUSE, m_ColorDiffuse);
	Xml::LitCouleur3fv(element, Xml::SPECULAR, m_ColorSpecular);

	// Position
	Xml::LitPosition3fv(element, Xml::POSITION, m_Position);

	// Direction
	Xml::LitDirection3fv(element, Xml::DIRECTION, m_Direction);

	// Fall off
	m_fFallOff = (float)Xml::LitValeur(element, Xml::FALLOFF);

	return true;
}

bool CLightTarget::LitFichier( CIfstreamMap &fichier )
{
	string mot;

	fichier >> mot;
	if( mot!="CouleurAmbient" )
		return false;
	fichier >> m_ColorAmbient[0] >> m_ColorAmbient[1] >> m_ColorAmbient[2];

	fichier >> mot;
	if( mot!="CouleurDiffuse" )
		return false;
	fichier >> m_ColorDiffuse[0] >> m_ColorDiffuse[1] >> m_ColorDiffuse[2];

	fichier >> mot;
	if( mot!="CouleurSpecular" )
		return false;
	fichier >> m_ColorSpecular[0] >> m_ColorSpecular[1] >> m_ColorSpecular[2];

	fichier >> mot;
	if( mot!="Position" )
		return false;
	fichier >> m_Position[0] >> m_Position[1] >> m_Position[2];

	fichier >> mot;
	if( mot!="Direction" )
		return false;
	fichier >> m_Direction[0] >> m_Direction[1] >> m_Direction[2];

	fichier >> mot;
	if( mot!="FallOff" )
		return false;
	fichier >> m_fFallOff;

	return true;
}

bool CLightTarget::SaveFichierMap( ofstream &fichier )
{
	fichier << "\n\nLumiereTarget";
	fichier << "\n\tCouleurAmbient\t\t" << m_ColorAmbient[0] << "\t" << m_ColorAmbient[1] << "\t"  << m_ColorAmbient[2];
	fichier << "\n\tCouleurDiffuse\t\t" << m_ColorDiffuse[0] << "\t" << m_ColorDiffuse[1] << "\t"  << m_ColorDiffuse[2];
	fichier << "\n\tCouleurSpecular\t\t" << m_ColorSpecular[0] << "\t" << m_ColorSpecular[1] << "\t"  << m_ColorSpecular[2];
	fichier << "\n\tPosition\t" << m_Position[0] << "\t" << m_Position[1] << "\t" << m_Position[2];
	fichier << "\n\tDirection\t" << m_Direction[0] << "\t" << m_Direction[1] << "\t" << m_Direction[2];
	fichier << "\n\tFallOff\t\t" << m_fFallOff;

	return true;
}

bool CLightTarget::Save(TiXmlElement* element)
{
	// Nom, r�f�rence, type
	TiXmlElement* elLum = new TiXmlElement(Xml::LUMIERE);
	elLum->SetAttribute(Xml::REF, m_refLight);
	elLum->SetAttribute(Xml::TYPE, Xml::TARGET);
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

void CLightTarget::EchangeXY()		// Echange les coordonn�es X et Y de l'objet
{
	CLight::EchangeXY();

	float var = m_Direction[0];
	m_Direction[0] = m_Direction[1];
	m_Direction[1] = var;
}
void CLightTarget::EchangeXZ()			// Echange les coordonn�es X et Z de l'objet
{
	CLight::EchangeXZ();

	float var = m_Direction[0];
	m_Direction[0] = m_Direction[2];
	m_Direction[2] = var;
}

void CLightTarget::EchangeYZ()			// Echange les coordonn�es Y et Z de l'objet
{
	CLight::EchangeYZ();

	float var = m_Direction[1];
	m_Direction[1] = m_Direction[2];
	m_Direction[2] = var;
}

void CLightTarget::Scale( float scaleX, float scaleY, float scaleZ )	// Homoth�tie pond�r�e selon X, Y et Z de l'objet
{
	CLight::Scale( scaleX, scaleY, scaleZ );

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
