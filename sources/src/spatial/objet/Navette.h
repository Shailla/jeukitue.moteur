#ifndef __JKT__NAVETTE_H
#define __JKT__NAVETTE_H

#include <vector>

#include "spatial/basic/Refreshable.h"

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
	float m_Vitesse;	// Vitesse de la navette à partir de ce point
	jkt::CV3D m_Position;	// Position de ce point du trajet de la navette

	CPointNavette( const CPointNavette& pp );
	CPointNavette();
	~CPointNavette();

	void operator =( const CPointNavette &pp );
	void LitFichierPoint( CIfstreamMap &fichier );
	void SaveFichierPoint( ostream &fichier );
	void SavePoint(TiXmlElement* element);
};

class CNavette : public CGeoObject {
	vector<CPointNavette> m_ListePoints;	// Liste des points sur le trajet de la navette
	jkt::CV3D m_Direction;	// Direction courante de la navette
	int m_Point;		// Point du trajet à utiliser
	float m_distPoints;	// Distance entre les deux points courants
	float m_Vitesse;	// Vitesse courante de la navette
	float m_Deplacement;	// Déplacement relatif courant de la navette
	jkt::CV3D m_Position;	// Position courante de la navette à partir du point courant intermédiaire

	int prochainPoint( int i );	// Prends le prochain point du trajet et calcule les paramètres

public:
	static const char* identifier;

	CNavette(CMap *map);
	~CNavette();

	void init() throw(CErreur) override;					// Initialisation de l'objet géométrique

	void Affiche() override;					// Fonction d'affichage de l'objet géométrique
	void refresh( CGame *game ) override;	// Rafraichissement des données, position, vitesse, ... de l'objet

	bool Save(TiXmlElement* element) override;

	float GereLaserPlayer(float pos[3], jkt::CV3D &Dir, float dist) override;
	void GereContactPlayer(float positionPlayer[3], CPlayer *player) override;
};

}	// JktMoteur

#endif

