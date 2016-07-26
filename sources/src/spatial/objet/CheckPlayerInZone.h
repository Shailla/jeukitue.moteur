/*
 * CheckPlayerInZone.h
 *
 *  Created on: 24 juil. 2016
 *      Author: Erwin
 */

#ifndef SRC_SPATIAL_OBJET_CHECKPLAYERINZONE_H_
#define SRC_SPATIAL_OBJET_CHECKPLAYERINZONE_H_

#include <string>

#include "spatial/Map.h"
#include "spatial/geo/Geo.h"

namespace jkt
{

class CheckPlayerInZone : public CGeo {
	// Identifiant du check, permet � un plugin par exemple de savoir quel check a �t� activ�
	std::string _id;

	// Permet d'activer l'affichage dans la Map de la zone de d�tection (� fin de debug essentiellement)
	bool _isVisible;

	// Zone dans laquelle un joueur d�clenche la d�tection
	float _zoneDetectionXmin;
	float _zoneDetectionXmax;
	float _zoneDetectionYmin;
	float _zoneDetectionYmax;
	float _zoneDetectionZmin;
	float _zoneDetectionZmax;
public:
	CheckPlayerInZone(CMap *map, const string& id, float zoneDetectionXmin, float zoneDetectionXmax, float zoneDetectionYmin, float zoneDetectionYmax, float zoneDetectionZmin, float zoneDetectionZmax);
	virtual ~CheckPlayerInZone();

	virtual void Affiche();		// Affiche l'objet g�om�trique
	void GereContactPlayer(float positionPlayer[3], CPlayer *player);

	void setVisibility(bool isVisible);
};

}

#endif /* SRC_SPATIAL_OBJET_CHECKPLAYERINZONE_H_ */
