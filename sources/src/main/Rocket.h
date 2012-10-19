
#ifndef __JKT__ROCKET_H
#define __JKT__ROCKET_H

#include "spatial/widget/Texture.h"
#include "util/V3D.h"
#include "spatial/Map.h"

#include "main/Projectil.h"

using namespace JktMoteur;

class CRocket:public CProjectil {
	enum ROCKET_STATE {
		ROCKET_STATE_DEPL,
		ROCKET_STATE_CONTACT,
		ROCKET_STATE_EXPLOSION,
	};

	static bool m_B_INIT_CLASSE;		// Indique si la classe a été initialisée
	static Texture* _textureExplosion;			// Texture de l'explosion de la rocket
	static JktMoteur::CMap *pMapRocket;	// Map associée à l'image de la rocket

	CV3D m_Pos;		// Position de la rocket
	CV3D m_Dir;		// Direction de la rocket
	float m_Teta;
	float m_Phi;

	ROCKET_STATE m_State;
	float m_Taille;

	void Affiche_S1();
	void Affiche_S2();
	void Deplace();		// Déplace la rocket, lorsqu'elle est à m_State=1

public:
	static bool INIT_CLASSE();	// Initialisation de la classe

	CRocket( CPlayer *player );
	~CRocket();

	void Affiche();
	bool Refresh();
};

#endif
