#ifndef __JKT__LASER_H
#define __JKT__LASER_H

#include "Projectil.h"

class CLaser:public CProjectil
{
	Uint32 m_TimeStart;	// Horloge au d�marrage du lancement de rayon laser
	float m_Delta;

public:
	CLaser( CPlayer *player );
	~CLaser();

	void Affiche();
	bool Refresh();
};

#endif