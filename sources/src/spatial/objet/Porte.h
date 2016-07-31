#ifndef __JKT__PORTE_H
#define __JKT__PORTE_H

#include "util/V3D.h"
#include "spatial/basic/MapObject.h"

class CGame;
class CPlayer;

namespace jkt
{

class CMap;
class CIstreamMap;

class CPorte : public CGeoObject {
	jkt::CV3D m_Position;		// Position de la porte
	float m_fMarge;					// Distance parcourue par la porte lors de son ouverture
	float m_fOuverture;				// Ouverture courante de la porte ( 0.0f <= m_fOuverture <= m_fMarge )
	float m_fVitesse;				// Vitesse d'ouverture de la porte (en unité float/milliseconde)
	float m_fDeclenche;				// Distance de la porte qui déclenche son mécanisme

public:
	static const char* identifier;
	CPorte( CMap *map );
	~CPorte();

	void init() throw(CErreur) override;					// Initialisation de l'objet géométrique

	void Affiche() override;				// Fonction d'affichage de l'objet géométrique
	void refresh( CGame *game ) override;	// Rafraichissement des données, position, vitesse, ... de l'objet

	bool Save(TiXmlElement* element) override;

	void GereContactPlayer(float positionPlayer[3], CPlayer *player ) override;
	float GereLaserPlayer( float pos[3], jkt::CV3D &Dir, float dist ) override;
};

}	// JktMoteur

#endif

