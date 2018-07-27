/*
 * CheckPlayerInZone.cpp
 *
 *  Created on: 24 juil. 2016
 *      Author: Erwin
 */

#include <agar/core.h>
#include <agar/gui.h>
#include <agar/dev.h>

#include "main/Fabrique.h"
#include "util/GLUtils.h"
#include "util/Trace.h"
#include "menu/Controller.h"
#include "main/Game.h"
#include "main/Player.h"
#include "main/event/PlayerZoneEvent.h"

#include "spatial/objet/CheckPlayerInZone.h"

using namespace std;

extern CGame Game;

namespace jkt {

const char* CheckPlayerInZone::identifier = "PlayerDetector";

CheckPlayerInZone::CheckPlayerInZone(	CMap* map, const string& id, float zoneDetectionXmin, float zoneDetectionXmax, float zoneDetectionYmin,
										float zoneDetectionYmax, float zoneDetectionZmin, float zoneDetectionZmax) : MapObject(map, MapObject::CHECK) {
	_id = id;

	_isVisible = false;
	_playerInZone = false;

	_zoneDetectionXmin = (zoneDetectionXmin <= zoneDetectionXmax) ? zoneDetectionXmin : zoneDetectionXmax;
	_zoneDetectionXmax = (zoneDetectionXmin >= zoneDetectionXmax) ? zoneDetectionXmin : zoneDetectionXmax;

	_zoneDetectionYmin = (zoneDetectionYmin <= zoneDetectionYmax) ? zoneDetectionYmin : zoneDetectionYmax;
	_zoneDetectionYmax = (zoneDetectionYmin >= zoneDetectionYmax) ? zoneDetectionYmin : zoneDetectionYmax;

	_zoneDetectionZmin = (zoneDetectionZmin <= zoneDetectionZmax) ? zoneDetectionZmin : zoneDetectionZmax;
	_zoneDetectionZmax = (zoneDetectionZmin >= zoneDetectionZmax) ? zoneDetectionZmin : zoneDetectionZmax;
}

CheckPlayerInZone::CheckPlayerInZone(CMap* map, const CheckPlayerInZone& other) : MapObject(map, MapObject::CHECK) {
	_id = other._id;

	_isVisible = other._isVisible;
	_playerInZone = other._playerInZone;

	_zoneDetectionXmin = other._zoneDetectionXmin;
	_zoneDetectionXmax = other._zoneDetectionXmax;
	_zoneDetectionYmin = other._zoneDetectionYmin;
	_zoneDetectionYmax = other._zoneDetectionYmax;
	_zoneDetectionZmin = other._zoneDetectionZmin;
	_zoneDetectionZmax = other._zoneDetectionZmax;
}

CheckPlayerInZone::~CheckPlayerInZone() {
}

MapObject* CheckPlayerInZone::clone() {
	return new CheckPlayerInZone(*this);
}

void CheckPlayerInZone::beginRefresh() {
}

bool CheckPlayerInZone::checkPositionInZone(float position[3]) {
	bool playerInZone = false;

	if(_zoneDetectionXmin < position[0] && position[0] < _zoneDetectionXmax) {
		if(_zoneDetectionYmin < position[1] && position[1] < _zoneDetectionYmax) {
			if(_zoneDetectionZmin < -position[2] && -position[2] < _zoneDetectionZmax) {
				playerInZone = true;
			}
		}
	}

	return playerInZone;
}

void CheckPlayerInZone::refresh(CGame* game) {
	int curseur = -1;
	CPlayer* player;
	float position[3];
	bool isNow, wasBefore;
	set<CPlayer*> playersInZoneNow;

	while((player = Game.nextPlayer(curseur))) {
		player->getPosition(position);

		if(checkPositionInZone(position)) {
			playersInZoneNow.insert(player);
		}
	}

	set<CPlayer*> allPlayers;
	allPlayers.insert(playersInZoneNow.begin(), playersInZoneNow.end());
	allPlayers.insert(_playersInZone.begin(), _playersInZone.end());


	for(CPlayer* player : allPlayers) {
		isNow = playersInZoneNow.find(player) != playersInZoneNow.end();
		wasBefore = _playersInZone.find(player) != _playersInZone.end();

		if(isNow && !wasBefore) {		// Entering in zone ?
			PlayerZoneEvent evt(Controller::Action::PlayerZoneDetectorActivated, player);
			Controller::executeAction(evt);
		}
		else if(!isNow && wasBefore) {	// Outgoing of zone
			PlayerZoneEvent evt(Controller::Action::PlayerZoneDetectorUnactivated, player);
			Controller::executeAction(evt);
		}
	}

	// Mise � jour de la liste des joeurs actuellement dans la zone
	_playersInZone.clear();
	_playersInZone.insert(playersInZoneNow.begin(), playersInZoneNow.end());
}

void CheckPlayerInZone::endRefresh() {
}

void CheckPlayerInZone::affiche() {
	if(_isVisible) {
		glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE );

		glColor4f(0.0f, 0.2f, 1.0f, 0.4f);

		Fabrique::getGlUtils()->drawCube(_zoneDetectionXmin, _zoneDetectionXmax, _zoneDetectionYmin, _zoneDetectionYmax, _zoneDetectionZmin, _zoneDetectionZmax);

		glDisable( GL_BLEND );
	}
}

void CheckPlayerInZone::afficheHighlighted(float r,float v,float b) {
	glColor3f(r, v, b);
	affiche();
}

bool CheckPlayerInZone::isPlayerInZone() const {
	return _playerInZone;
}

const char* CheckPlayerInZone::toString() {
	ostringstream ttt;
	ttt << identifier << " id=" << _id;

	_tostring = ttt.str();

	return _tostring.c_str();
}

void CheckPlayerInZone::init() throw(CErreur) {
}

void CheckPlayerInZone::initGL() {
}

void CheckPlayerInZone::freeGL() {
}

void CheckPlayerInZone::setVisibility(bool isVisible) {
	_isVisible = isVisible;
}

bool CheckPlayerInZone::Lit(TiXmlElement* el, CMap& map, MapLogger* mapLogger) throw(CErreur) {
	LOGWARN(("Use of not implemented method"));
	return true;
}

bool CheckPlayerInZone::Save(TiXmlElement* element) throw(CErreur) {
	LOGWARN(("Use of not implemented method"));
	return true;
}

void CheckPlayerInZone::echangeXY() {	// Echange les axes X et Y de l'objet
	float var1, var2;

	var1 = _zoneDetectionXmin;
	var2 = _zoneDetectionXmax;

	_zoneDetectionXmin = _zoneDetectionYmin;
	_zoneDetectionXmax = _zoneDetectionYmax;

	_zoneDetectionYmin = var1;
	_zoneDetectionYmax = var2;
}

void CheckPlayerInZone::echangeXZ() {	// Echange les axes X et Y de l'objet
	float var1, var2;

	var1 = _zoneDetectionXmin;
	var2 = _zoneDetectionXmax;

	_zoneDetectionXmin = _zoneDetectionZmin;
	_zoneDetectionXmax = _zoneDetectionZmax;

	_zoneDetectionZmin = var1;
	_zoneDetectionZmax = var2;
}

void CheckPlayerInZone::echangeYZ() {	// Echange les axes X et Y de l'objet
	float var1, var2;

	var1 = _zoneDetectionYmin;
	var2 = _zoneDetectionYmax;

	_zoneDetectionYmin = _zoneDetectionZmin;
	_zoneDetectionYmax = _zoneDetectionZmax;

	_zoneDetectionZmin = var1;
	_zoneDetectionZmax = var2;
}

void CheckPlayerInZone::scale( float scaleX, float scaleY, float scaleZ ) {
	_zoneDetectionXmin *= scaleX;
	_zoneDetectionXmax *= scaleX;

	_zoneDetectionYmin *= scaleY;
	_zoneDetectionYmax *= scaleY;

	_zoneDetectionZmin *= scaleZ;
	_zoneDetectionZmax *= scaleZ;
}

void CheckPlayerInZone::translate( float x, float y, float z ) {
	_zoneDetectionXmin += x;
	_zoneDetectionXmax += x;

	_zoneDetectionYmin += y;
	_zoneDetectionYmax += y;

	_zoneDetectionZmin += z;
	_zoneDetectionZmax += z;
}

}
