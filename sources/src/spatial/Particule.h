
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
	/** Accélération actuelle de la particule. */
	JktUtils::CV3D acceleration;

	/** Temps auquel la particule a été créée ou réinitialisée. */
	unsigned int date2naissance;
	/** Durée que vivra la particule avant d'être réinitialisée. */
	unsigned int duree2vie;
	/** Indique si la particule est visible ou non. */
	bool visible;

	// Constructeurs
	CParticule();
	CParticule(CV3D pos, CV3D vit, float m, unsigned int duree);

	// Fonctions
	void Calcule();							// Calcule paramètres (position,...)
	bool Vie(unsigned int temps);			// Vérifie si la particule est encore en vie

	/**
	 * Réinitialise la particule et lui redonne la vie.
	 * @param position nouvelle position de la particule
	 * @param vitesse nouvelle vitesse de la particule
	 * @param now temps courant
	 */
	void Reset(CV3D pos, CV3D vit, unsigned int now);			// Réinitialise la particule à une nouvelle position
};

}	// JktMoteur

#endif
