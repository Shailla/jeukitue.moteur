
#include <fstream>

using namespace std;

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

namespace jkt
{

CLightOmni::CLightOmni(CMap* map) : CLight(map)
{
}

CLightOmni::~CLightOmni()
{
}

bool CLightOmni::LitFichier( CIfstreamMap &fichier )
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

	return true;
}

bool CLightOmni::SaveFichierMap( ofstream &fichier )
{
	fichier << "\n\nLumiereOmni";
	fichier << "\n\tCouleurAmbient\t\t" << m_ColorAmbient[0] << "\t" << m_ColorAmbient[1] << "\t"  << m_ColorAmbient[2];
	fichier << "\n\tCouleurDiffuse\t\t" << m_ColorDiffuse[0] << "\t" << m_ColorDiffuse[1] << "\t"  << m_ColorDiffuse[2];
	fichier << "\n\tCouleurSpecular\t\t" << m_ColorSpecular[0] << "\t" << m_ColorSpecular[1] << "\t"  << m_ColorSpecular[2];
	fichier << "\n\tPosition\t" << m_Position[0] << "\t" << m_Position[1] << "\t" << m_Position[2];

	return true;
}

bool CLightOmni::Save(TiXmlElement* element)
{
	// Nom, référence, type
	TiXmlElement* elLum = new TiXmlElement(Xml::LUMIERE);
	elLum->SetAttribute(Xml::REF, m_refLight);
	elLum->SetAttribute(Xml::TYPE, Xml::OMNI);
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

void CLightOmni::Active()
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

	glEnable( GL_LIGHTING );

	glPopMatrix();

	CLight::Active();

	glLightfv( m_refLight, GL_POSITION, fff );
	glLightfv( m_refLight, GL_AMBIENT, m_ColorAmbient );
	glLightfv( m_refLight, GL_DIFFUSE, m_ColorDiffuse );
	glLightfv( m_refLight, GL_SPECULAR, m_ColorSpecular );
}

bool CLightOmni::Lit(TiXmlElement* element, MapLogger* mapLogger) {
	double ref;
	const char* type;

	// Référence
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

	if(strcmp(Xml::OMNI, type)) {
		mapLogger->logError("Fichier Map corrompu : Type incompatible");
		throw CErreur("Fichier Map corrompu : Type incompatible");
	}

	// Couleurs
	Xml::LitCouleur3fv(element, Xml::AMBIANTE, m_ColorAmbient);
	Xml::LitCouleur3fv(element, Xml::DIFFUSE, m_ColorDiffuse);
	Xml::LitCouleur3fv(element, Xml::SPECULAR, m_ColorSpecular);

	// Position
	Xml::LitPosition3fv(element, Xml::POSITION, m_Position);

	return true;
}

void CLightOmni::Desactive()
{
	CLight::Desactive();
}

}	// JktMoteur
