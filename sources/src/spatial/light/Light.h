
#ifndef __JKT__LIGHT_H
#define __JKT__LIGHT_H

#include <iostream>

#include "tinyxml.h"

#include "spatial/basic/MapObject.h"
#include "spatial/MapLogger.h"

namespace jkt
{

class CIfstreamMap;

class CLight : public MapObject {
	static int m_nbrLightActivated;
	static bool m_bLight0;
	static bool m_bLight1;
	static bool m_bLight2;
	static bool m_bLight3;
	static bool m_bLight4;
	static bool m_bLight5;
	static bool m_bLight6;
	static bool m_bLight7;

protected:
	static const char* LUMIERE;
	static const char* TARGET;
	static const char* OMNI;
	static const char* FALLOFF;
	static const char* POSITION;
	static const char* DIRECTION;
	static const char* X;
	static const char* Y;
	static const char* Z;
	static const char* VALEUR;
	static const char* R;
	static const char* G;
	static const char* B;
	static const char* AMBIANTE;
	static const char* DIFFUSE;
	static const char* SPECULAR;
	static const char* TYPE;
	static const char* REF;

	float m_ColorAmbient[4];	// Couleur de la lumi�re
	float m_ColorDiffuse[4];	// Couleur de la lumi�re
	float m_ColorSpecular[4];	// Couleur de la lumi�re
	float m_Position[4];		// Position de la lumi�re
	int m_refLight;				// R�f�rence sur la lumi�re openGL actuellement utilis�e
	int takeLight();			// Trouve une lumi�re openGL disponible et la r�serve
	void freeLight( int ref );	// Lib�re une lumi�re openGL r�serv�e

public :
	CLight(CMap* map);
	virtual ~CLight();

	// M�thodes de s�rialisation
	virtual bool Lit(TiXmlElement* element, CMap& map, MapLogger* mapLogger) = 0;		// Lit une light dans un fichier Map format XML
	virtual bool LitFichier(CIfstreamMap &fichier) = 0;						// Lit une light dans un fichier Map
	virtual bool SaveFichierMap(std::ofstream &fichier) = 0;				// Sauve la light dans un fichier Map
	virtual bool Save(TiXmlElement* element) = 0;							// Sauve la light dans un fichier Map format XML

	// M�thodes de manipulation g�om�trique
	virtual void EchangeXY() override;										// Echange les coordonn�es X et Y de l'objet
	virtual void EchangeXZ() override;										// Echange les coordonn�es X et Z de l'objet
	virtual void EchangeYZ() override;										// Echange les coordonn�es Y et Z de l'objet
	virtual void Scale(float scaleX, float scaleY, float scaleZ) override;	// Homoth�tie pond�r�e selon X, Y et Z de l'objet
	virtual void translate(float x, float y, float z) override;				// Translation pond�r�e selon X, Y et Z de l'objet

	virtual void Active();		// Active la lumi�re
	virtual void Desactive();	// D�sactive la lumi�re

	void SetPosition(const float position[4]);
	void SetColorAmbient(const float color[4]);
	void SetColorDiffuse(const float color[4]);
	void SetColorSpecular(const float color[4]);
	void SetColor(const float color[4]);

	int getRefLight() const;
	void getPosition(float position[4]) const;
	void getCouleurAmbiante(float couleurAmbient[4]) const;
	void getCouleurDiffuse(float couleurDiffuse[4]) const;
	void getCouleurSpeculaire(float couleurSpeculaire[4]) const;
};

}	// JktMoteur

#endif

