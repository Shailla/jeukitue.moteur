#ifndef __JKT__NAVETTE_H
#define __JKT__NAVETTE_H

#include <vector>

using namespace std;

#include "spatial/Mouve.h"

#include "util/V3D.h"

class CGame;

namespace jkt
{

class CMap;
class CIstreamMap;

class CPointNavette
{
public:
	static const char* identifier;
	float m_Vitesse;	// Vitesse de la navette � partir de ce point
	jkt::CV3D m_Position;	// Position de ce point du trajet de la navette

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
	jkt::CV3D m_Direction;	// Direction courante de la navette
	int m_Point;		// Point du trajet � utiliser
	float m_distPoints;	// Distance entre les deux points courants
	float m_Vitesse;	// Vitesse courante de la navette
	float m_Deplacement;	// D�placement relatif courant de la navette
	jkt::CV3D m_Position;	// Position courante de la navette � partir du point courant interm�diaire

	int prochainPoint( int i );	// Prends le prochain point du trajet et calcule les param�tres

public:
	static const char* identifier;

	CNavette( CMap *map );
	~CNavette();

	void Init();					// Initialisation de l'objet g�om�trique

	void Affiche();					// Fonction d'affichage de l'objet g�om�trique
	void Refresh( CGame *game );	// Rafraichissement des donn�es, position, vitesse, ... de l'objet

	bool Lit(TiXmlElement* el) {return true;}
	//bool LitFichier( CIfstreamMap &fichier );		// Lit un objet g�o dans un fichier Map
	//void LitFichierNavette( CIfstreamMap &fichier );	// Lit un objet g�o dans un fichier Map
	//bool SaveNameType( ofstream &fichier );				// Sauve le nom du type d'objet dans le fichier
	//bool SaveFichierMap( ofstream &fichier );			// Sauve l'objet g�o dans un fichier Map
	bool Save(TiXmlElement* element);

	float GereLaserPlayer(float pos[3], jkt::CV3D &Dir, float dist);
	void GereContactPlayer(float positionPlayer[3], CPlayer *player);
};

}	// JktMoteur

#endif

