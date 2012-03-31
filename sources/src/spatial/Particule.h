
#ifndef __JKT__PARTICULE_H
#define __JKT__PARTICULE_H

#include "util/V3D.h"

using namespace JktUtils;

namespace JktMoteur
{

class CParticule
{
public:
		// Paramètres
	CV3D position;
	CV3D vitesse;
	CV3D acceleration;
	float masse;					// Masse de la particule
	unsigned int date2naissance;	// Instant de création de la particule
	unsigned int duree2vie;			// Durée de vie de la particule
	bool visible;					// Indique si la particule est visible ou non
	unsigned int number;	// DEBUG
		// Constructeurs
	CParticule();
	CParticule( CV3D pos, CV3D vit, float m, unsigned int duree );

		// Fonctions
	void Calcule();		// Calcule paramètres (position,...)
	bool Vie( unsigned int temps );			// Vérifie si la particule est encore en vie
	void Reset( CV3D pos, CV3D vit );			// Réinitialise la particule à une nouvelle position
};

}	// JktMoteur

#endif
