#ifndef __JKT__NAVETTE_H
#define __JKT__NAVETTE_H

#include "spatial/Mouve.h"

#include "util/V3D.h"

class CGame;

namespace JKT_PACKAGE_MOTEUR3D
{

class CMap;
class CIstreamMap;

class CPointNavette
{
public:
	static const char* identifier;
	float m_Vitesse;	// Vitesse de la navette à partir de ce point
	CV3D m_Position;	// Position de ce point du trajet de la navette

	CPointNavette( const CPointNavette& pp );
	CPointNavette();
	~CPointNavette();

	void operator =( const CPointNavette &pp );
	void LitFichierPoint( CIfstreamMap &fichier );
	void SaveFichierPoint( ostream &fichier );
	void SavePoint(TiXmlElement* element);
};

class CNavette:public CGeoObject, public CMouve
{
	vector<CPointNavette> m_ListePoints;	// Liste des points sur le trajet de la navette
	CV3D m_Direction;	// Direction courante de la navette
	int m_Point;		// Point du trajet à utiliser
	float m_distPoints;	// Distance entre les deux points courants
	float m_Vitesse;	// Vitesse courante de la navette
	float m_Deplacement;	// Déplacement relatif courant de la navette
	CV3D m_Position;	// Position courante de la navette à partir du point courant intermédiaire

	int prochainPoint( int i );	// Prends le prochain point du trajet et calcule les paramètres

public:
	static const char* identifier;

	CNavette( CMap *map );
	~CNavette();

	void Init();					// Initialisation de l'objet géométrique

	void Affiche();					// Fonction d'affichage de l'objet géométrique
	void Refresh( CGame *game );	// Rafraichissement des données, position, vitesse, ... de l'objet

	bool Lit(TiXmlElement* el) {return true;}
	//bool LitFichier( CIfstreamMap &fichier );		// Lit un objet géo dans un fichier Map
	//void LitFichierNavette( CIfstreamMap &fichier );	// Lit un objet géo dans un fichier Map
	//bool SaveNameType( ofstream &fichier );				// Sauve le nom du type d'objet dans le fichier
	//bool SaveFichierMap( ofstream &fichier );			// Sauve l'objet géo dans un fichier Map
	bool Save(TiXmlElement* element);

	float GereLaser( float pos[3], CV3D &Dir, float dist );
	void GereContactPlayer( const float pos[3], CPlayer *player );
};

}	// JKT_PACKAGE_MOTEUR3D

#endif

