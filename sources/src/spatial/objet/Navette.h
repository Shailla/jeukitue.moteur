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
	float m_Vitesse;	// Vitesse de la navette � partir de ce point
	jkt::CV3D m_Position;	// Position de ce point du trajet de la navette

	CPointNavette( const CPointNavette& pp );
	CPointNavette();
	~CPointNavette();

	void operator =( const CPointNavette &pp );
	void LitFichierPoint( CIfstreamMap &fichier );
	void SaveFichierPoint( std::ostream &fichier );
	void SavePoint(TiXmlElement* element);
};

class CNavette : public CGeoObject {
	std::vector<CPointNavette> m_ListePoints;	// Liste des points sur le trajet de la navette
	jkt::CV3D m_Direction;	// Direction courante de la navette
	int m_Point;		// Point du trajet � utiliser
	float m_distPoints;	// Distance entre les deux points courants
	float m_Vitesse;	// Vitesse courante de la navette
	float m_Deplacement;	// D�placement relatif courant de la navette
	jkt::CV3D m_Position;	// Position courante de la navette � partir du point courant interm�diaire

	int prochainPoint( int i );	// Prends le prochain point du trajet et calcule les param�tres

public:
	static const char* identifier;

	CNavette(CMap *map);
	~CNavette();

	void init() noexcept(false) override;					// Initialisation de l'objet g�om�trique

	void affiche() override;					// Fonction d'affichage de l'objet g�om�trique
	void refresh( CGame *game ) override;	// Rafraichissement des donn�es, position, vitesse, ... de l'objet

	bool Save(TiXmlElement* element) noexcept(false) override;

	float gereLaserPlayer(float pos[3], jkt::CV3D &Dir, float dist) override;
	void gereContactPlayer(float positionPlayer[3], CPlayer *player) override;
};

}	// JktMoteur

#endif

