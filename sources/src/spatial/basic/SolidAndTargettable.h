/*
 * SolidAndTargettable.h
 *
 *  Created on: 29 juil. 2016
 *      Author: VGDJ7997
 */

#ifndef SRC_SPATIAL_BASIC_SOLIDANDTARGETTABLE_H_
#define SRC_SPATIAL_BASIC_SOLIDANDTARGETTABLE_H_

#include "util/V3D.h"
#include "main/Player.h"

namespace jkt {

class SolidAndTargettable {
public:
	SolidAndTargettable();
	SolidAndTargettable(const SolidAndTargettable& other);
	virtual ~SolidAndTargettable();

	// Gestion des contacts
	virtual void GereContactPlayer(float positionPlayer[3], CPlayer *player) = 0;	// Gère les contact entre 'player' et l'objet géo
	virtual float GereLaserPlayer(float pos[3], CV3D& Dir, float dist) = 0;		// Voir la définition de la fonction
	virtual bool checkContact(const float pos[3], float dist);
};

} /* namespace jkt */

#endif /* SRC_SPATIAL_BASIC_SOLIDANDTARGETTABLE_H_ */
