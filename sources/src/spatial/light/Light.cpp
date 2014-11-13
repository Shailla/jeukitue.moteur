
#include <string>
#include <fstream>
#include <iostream>
#ifdef WIN32
	#include <windows.h>
#endif
#include <GL/gl.h>

#include "util/Trace.h"

using namespace std;

#include "spatial/light/Light.h"

namespace JktMoteur
{

int CLight::m_nbrLightActivated = 0;	// Nombre de lumières actuellement activées

bool CLight::m_bLight0 = false;
bool CLight::m_bLight1 = false;
bool CLight::m_bLight2 = false;
bool CLight::m_bLight3 = false;
bool CLight::m_bLight4 = false;
bool CLight::m_bLight5 = false;
bool CLight::m_bLight6 = false;
bool CLight::m_bLight7 = false;

int CLight::takeLight()
{
	if( !m_bLight0 )
	{
		m_bLight0 = true;
		return GL_LIGHT0;
	}
	else if( !m_bLight1 )
	{
		m_bLight1 = true;
		return GL_LIGHT1;
	}
	else if( !m_bLight2 )
	{
		m_bLight2 = true;
		return GL_LIGHT2;
	}
	else if( !m_bLight3 )
	{
		m_bLight3 = true;
		return GL_LIGHT3;
	}
	else if( !m_bLight4 )
	{
		m_bLight4 = true;
		return GL_LIGHT4;
	}
	else if( !m_bLight5 )
	{
		m_bLight5 = true;
		return GL_LIGHT5;
	}
	else if( !m_bLight6 )
	{
		m_bLight6 = true;
		return GL_LIGHT6;
	}
	else if( !m_bLight7 )
	{
		m_bLight7 = true;
		return GL_LIGHT7;
	}
	else
	{
		TRACE().p( TRACE_ERROR, "Clight::findFreeLight() : Aucune lumiere n'est disponible" );
		return 0;
	}
}

void CLight::freeLight( int ref )
{
	switch( ref )
	{
	case GL_LIGHT0:
		m_bLight0 = false;
		break;
	case GL_LIGHT1:
		m_bLight1 = false;
		break;
	case GL_LIGHT2:
		m_bLight2 = false;
		break;
	case GL_LIGHT3:
		m_bLight3 = false;
		break;
	case GL_LIGHT4:
		m_bLight4 = false;
		break;
	case GL_LIGHT5:
		m_bLight5 = false;
		break;
	case GL_LIGHT6:
		m_bLight6 = false;
		break;
	case GL_LIGHT7:
		m_bLight7 = false;
		break;
	}
}

CLight::CLight()
{
	m_ColorAmbient[0] = 0.0f;		m_ColorAmbient[1] = 0.0f;		m_ColorAmbient[2] = 0.0f;		m_ColorAmbient[3] = 1.0f;
	m_ColorDiffuse[0] = 0.0f;		m_ColorDiffuse[1] = 0.0f;		m_ColorDiffuse[2] = 0.0f;		m_ColorDiffuse[3] = 1.0f;
	m_ColorSpecular[0] = 0.0f;		m_ColorSpecular[1] = 0.0f;		m_ColorSpecular[2] = 0.0f;		m_ColorSpecular[3] = 1.0f;

	m_Position[0] = 0.0f;	m_Position[1] = 0.0f;	m_Position[2] = 0.0f;	m_Position[3] = 1.0f;

	m_refLight = 0;
}

CLight::~CLight()
{
}

void CLight::EchangeXY()		// Echange les coordonnées X et Y de l'objet
{
	float var = m_Position[0];
	m_Position[0] = m_Position[1];
	m_Position[1] = var;
}
void CLight::EchangeXZ()			// Echange les coordonnées X et Z de l'objet
{
	float var = m_Position[0];
	m_Position[0] = m_Position[2];
	m_Position[2] = var;
}

void CLight::EchangeYZ()			// Echange les coordonnées Y et Z de l'objet
{
	float var = m_Position[1];
	m_Position[1] = m_Position[2];
	m_Position[2] = var;
}

void CLight::Scale( float scaleX, float scaleY, float scaleZ )	// Homothétie pondérée selon X, Y et Z de l'objet
{
	m_Position[ 0 ] *= scaleX;
	m_Position[ 1 ] *= scaleY;
	m_Position[ 2 ] *= scaleZ;
}

void CLight::translate( float x, float y, float z )	// Translation pondérée selon X, Y et Z de l'objet
{
	m_Position[ 0 ] += x;
	m_Position[ 1 ] += y;
	m_Position[ 2 ] += z;
}

void CLight::SetPosition( const float position[4] ) {
	m_Position[0] = position[0];
	m_Position[1] = position[1];
	m_Position[2] = position[2];
	m_Position[3] = position[3];

	cout << endl << "Position lumiere : " << position[0] << "  " << position[1] << "  " << position[2] << "  " << position[3] << endl;
}

void CLight::SetColorAmbient( const float color[4] ) {
	m_ColorAmbient[0] = color[0];
	m_ColorAmbient[1] = color[1];
	m_ColorAmbient[2] = color[2];
	m_ColorAmbient[3] = color[3];
}

void CLight::SetColorDiffuse( const float color[4] ) {
	m_ColorDiffuse[0] = color[0];
	m_ColorDiffuse[1] = color[1];
	m_ColorDiffuse[2] = color[2];
	m_ColorDiffuse[3] = color[3];
}

void CLight::SetColorSpecular( const float color[4] ) {
	m_ColorSpecular[0] = color[0];
	m_ColorSpecular[1] = color[1];
	m_ColorSpecular[2] = color[2];
	m_ColorSpecular[3] = color[3];
}

void CLight::SetColor( const float color[4] ) {
	SetColorAmbient(color);
	SetColorDiffuse(color);
	SetColorSpecular(color);
}

void CLight::Active() {
	if( !m_nbrLightActivated )		// Si toutes les lumières sont éteintes
		glEnable( GL_LIGHTING );	// allume l'éclairage

	m_nbrLightActivated++;			// Indique qu'une lumière de plus est allumée

	m_refLight = takeLight();		// Réserve une lumière
	glEnable( m_refLight );
}

void CLight::Desactive() {
	m_nbrLightActivated--;
	glDisable( m_refLight );

	if( !m_nbrLightActivated )
		glDisable( GL_LIGHTING );

	freeLight( m_refLight );		// Libère la lumière
}

int CLight::getRefLight() const {
	return m_refLight;
}

void CLight::getPosition(float position[4]) const {
	position[0] = m_Position[0];
	position[1] = m_Position[1];
	position[2] = m_Position[2];
}

void CLight::getCouleurAmbiante(float couleurAmbient[4]) const {
	couleurAmbient[0] = m_ColorAmbient[0];
	couleurAmbient[1] = m_ColorAmbient[1];
	couleurAmbient[2] = m_ColorAmbient[2];
}

void CLight::getCouleurDiffuse(float couleurDiffuse[4]) const {
	couleurDiffuse[0] = m_ColorDiffuse[0];
	couleurDiffuse[1] = m_ColorDiffuse[1];
	couleurDiffuse[2] = m_ColorDiffuse[2];
}

void CLight::getCouleurSpeculaire(float couleurSpeculaire[4]) const {
	couleurSpeculaire[0] = m_ColorSpecular[0];
	couleurSpeculaire[1] = m_ColorSpecular[1];
	couleurSpeculaire[2] = m_ColorSpecular[2];
}

}	// JktMoteur
