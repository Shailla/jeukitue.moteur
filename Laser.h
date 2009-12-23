#ifndef __JKT__LASER_H
#define __JKT__LASER_H

#include "Projectil.h"

class CLaser:public CProjectil
{
	Uint32 m_TimeStart;	// Horloge au démarrage du lancement de rayon laser
	float m_Delta;

public:
	CLaser( CPlayer *player );
	~CLaser();

	void Affiche();
	bool Refresh();
};

#endif
