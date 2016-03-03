
#ifndef __JKT__PARTICULE_H
#define __JKT__PARTICULE_H

#include "util/V3D.h"

using namespace JktUtils;

namespace JktMoteur
{

class CParticule {
public:
	/** Position actuelle de la particule. */
	JktUtils::CV3D position;
	/** Vitesse actuelle de la particule. */
	JktUtils::CV3D vitesse;
	/** Acc�l�ration actuelle de la particule. */
	JktUtils::CV3D acceleration;

	/** Temps auquel la particule a �t� cr��e ou r�initialis�e. */
	unsigned int date2naissance;
	/** Dur�e que vivra la particule avant d'�tre r�initialis�e. */
	unsigned int duree2vie;
	/** Indique si la particule est visible ou non. */
	bool visible;

	// Constructeurs
	CParticule();
	CParticule(CV3D pos, CV3D vit, float m, unsigned int duree);

	// Fonctions
	void Calcule();							// Calcule param�tres (position,...)
	bool Vie(unsigned int temps);			// V�rifie si la particule est encore en vie

	/**
	 * R�initialise la particule et lui redonne la vie.
	 * @param position nouvelle position de la particule
	 * @param vitesse nouvelle vitesse de la particule
	 * @param now temps courant
	 */
	void Reset(CV3D pos, CV3D vit, unsigned int now);			// R�initialise la particule � une nouvelle position
};

}	// JktMoteur

#endif
