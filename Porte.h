#ifndef __JKT__PORTE_H
#define __JKT__PORTE_H

#include "Mouve.h"

#include "V3D.h"

class CGame;
class CPlayer;

namespace JKT_PACKAGE_MOTEUR3D
{

class CMap;
class CIstreamMap;

class CPorte:public CGeoObject, public CMouve
{
	CV3D m_Position;		// Position de la porte
	float m_fMarge;			// Distance parcourue par la porte lors de son ouverture
	float m_fOuverture;		// Ouverture courante de la porte ( 0.0f <= m_fOuverture <= m_fMarge )
	float m_fVitesse;		// Vitesse d'ouverture de la porte (en unité float/milliseconde)
	float m_fDeclenche;		// Distance de la porte qui déclenche son mécanisme
	
public:
	static const char* identifier;
	CPorte( CMap *map );
	~CPorte();

	void Init();					// Initialisation de l'objet géométrique
	
	void Affiche();					// Fonction d'affichage de l'objet géométrique
	void Refresh( CGame *game );	// Rafraichissement des données, position, vitesse, ... de l'objet

	bool Lit(TiXmlElement* el) {return true;}
	//bool LitFichier( CIfstreamMap &fichier );	// Lit un objet géo dans un fichier Map
	//void LitFichierPorte( CIfstreamMap &fichier );	// Lit un objet géo dans un fichier Map
	//bool SaveNameType( ofstream &fichier );			// Sauve le nom du type d'objet dans le fichier
	//bool SaveFichierMap( ofstream &fichier );		// Sauve l'objet géo dans un fichier Map
	bool Save(TiXmlElement* element);

	void GereContactPlayer( const float pos[3], CPlayer *player );
	float GereLaser( float pos[3], CV3D &Dir, float dist );
};

}	// JKT_PACKAGE_MOTEUR3D

#endif

