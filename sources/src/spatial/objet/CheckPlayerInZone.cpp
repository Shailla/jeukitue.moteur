/*
 * CheckPlayerInZone.cpp
 *
 *  Created on: 24 juil. 2016
 *      Author: Erwin
 */

#include "main/Fabrique.h"
#include "util/GLUtils.h"

#include "spatial/objet/CheckPlayerInZone.h"

namespace jkt {

CheckPlayerInZone::CheckPlayerInZone(CMap *map, const string& id, float zoneDetectionXmin, float zoneDetectionXmax, float zoneDetectionYmin, float zoneDetectionYmax, float zoneDetectionZmin, float zoneDetectionZmax) : CGeo(map) {
	_id = id;

	_isVisible = false;

	_zoneDetectionXmin = zoneDetectionXmin;
	_zoneDetectionXmax = zoneDetectionXmax;
	_zoneDetectionYmin = zoneDetectionYmin;
	_zoneDetectionYmax = zoneDetectionYmax;
	_zoneDetectionZmin = zoneDetectionZmin;
	_zoneDetectionZmax = zoneDetectionZmax;
}

CheckPlayerInZone::~CheckPlayerInZone() {
}

void CheckPlayerInZone::Affiche() {
	if(_isVisible) {
		Fabrique::getGlUtils()->drawCube(_zoneDetectionXmin, _zoneDetectionXmax, _zoneDetectionYmin, _zoneDetectionYmax, _zoneDetectionZmin, _zoneDetectionXmax);
	}
}

void CheckPlayerInZone::GereContactPlayer(float positionPlayer[3], CPlayer *) {
	if(_zoneDetectionXmin < positionPlayer[0] && positionPlayer[0] < _zoneDetectionXmax) {
		if(_zoneDetectionYmin < positionPlayer[1] && positionPlayer[0] < _zoneDetectionYmax) {
			if(_zoneDetectionZmin < positionPlayer[2] && positionPlayer[0] < _zoneDetectionZmax) {
// TODO Générer un événement vers les plugin avec_ id comme identifiant
			}
		}
	}
}

void CheckPlayerInZone::setVisibility(bool isVisible) {
	_isVisible = isVisible;
}

}
