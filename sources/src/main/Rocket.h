
#ifndef __JKT__ROCKET_H
#define __JKT__ROCKET_H

#include "spatial/widget/Texture.h"
#include "util/V3D.h"
#include "spatial/Map.h"

#include "main/Projectil.h"

using namespace jkt;

class CRocket : public CProjectil {
	enum ROCKET_STATE {
		ROCKET_STATE_DEPL,
		ROCKET_STATE_CONTACT,
		ROCKET_STATE_EXPLOSION,
	};

	static bool m_B_INIT_CLASSE;		// Indique si la classe a �t� initialis�e
	static Texture* _textureExplosion;			// Texture de l'explosion de la rocket
	static jkt::CMap *_mapRocket;	// Map associ�e � l'image de la rocket

	CV3D m_Pos;		// Position de la rocket
	CV3D m_Dir;		// Direction de la rocket
	float m_Teta;
	float m_Phi;

	ROCKET_STATE m_State;
	float m_Taille;

	/** Affiche la rocket. */
	void afficheRocket();

	/** Affiche l'explosion de la rocket. */
	void afficheExplosion();

	// D�place la rocket si elle n'a pas encore explos� ou fait la exploser
	void Deplace();

public:
	static bool INIT_CLASSE();	// Initialisation de la classe

	CRocket( CPlayer *player );
	~CRocket();

	void Affiche() override;
	bool Refresh() override;
};

#endif
