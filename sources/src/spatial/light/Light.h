
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

	float m_ColorAmbient[4];	// Couleur de la lumière
	float m_ColorDiffuse[4];	// Couleur de la lumière
	float m_ColorSpecular[4];	// Couleur de la lumière
	float m_Position[4];		// Position de la lumière
	int m_refLight;				// Référence sur la lumière openGL actuellement utilisée
	int takeLight();			// Trouve une lumière openGL disponible et la réserve
	void freeLight( int ref );	// Libère une lumière openGL réservée

public :
	CLight(CMap* map);
	virtual ~CLight();

	// Méthodes de sérialisation
	virtual bool Lit(TiXmlElement* element, CMap& map, MapLogger* mapLogger) = 0;		// Lit une light dans un fichier Map format XML
	virtual bool LitFichier(CIfstreamMap &fichier) = 0;						// Lit une light dans un fichier Map
	virtual bool SaveFichierMap(std::ofstream &fichier) = 0;				// Sauve la light dans un fichier Map
	virtual bool Save(TiXmlElement* element) = 0;							// Sauve la light dans un fichier Map format XML

	// Méthodes de manipulation géométrique
	virtual void EchangeXY() override;										// Echange les coordonnées X et Y de l'objet
	virtual void EchangeXZ() override;										// Echange les coordonnées X et Z de l'objet
	virtual void EchangeYZ() override;										// Echange les coordonnées Y et Z de l'objet
	virtual void Scale(float scaleX, float scaleY, float scaleZ) override;	// Homothétie pondérée selon X, Y et Z de l'objet
	virtual void translate(float x, float y, float z) override;				// Translation pondérée selon X, Y et Z de l'objet

	virtual void Active();		// Active la lumière
	virtual void Desactive();	// Désactive la lumière

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

