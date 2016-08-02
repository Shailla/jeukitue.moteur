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
#include "menu/Controller.h"
#include "main/Game.h"
#include "main/Player.h"

#include "spatial/objet/CheckPlayerInZone.h"

extern CGame Game;

namespace jkt {

const char* CheckPlayerInZone::identifier = "PlayerDetector";

CheckPlayerInZone::CheckPlayerInZone(	CMap* map, const string& id, float zoneDetectionXmin, float zoneDetectionXmax, float zoneDetectionYmin,
		float zoneDetectionYmax, float zoneDetectionZmin, float zoneDetectionZmax) : Object(map) {
	_id = id;

	_isVisible = false;
	_playerInZone = false;

	_zoneDetectionXmin = zoneDetectionXmin;
	_zoneDetectionXmax = zoneDetectionXmax;
	_zoneDetectionYmin = zoneDetectionYmin;
	_zoneDetectionYmax = zoneDetectionYmax;
	_zoneDetectionZmin = zoneDetectionZmin;
	_zoneDetectionZmax = zoneDetectionZmax;
}

CheckPlayerInZone::CheckPlayerInZone(CMap* map, const CheckPlayerInZone& other) : Object(map) {
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

CheckPlayerInZone* CheckPlayerInZone::clone() const {
	return new CheckPlayerInZone(*this);
}

void CheckPlayerInZone::beginRefresh() {
}

bool CheckPlayerInZone::checkPositionInZone(float position[3]) {
	bool playerInZone = false;

	if(_zoneDetectionXmin < position[0] && position[0] < _zoneDetectionXmax) {
		if(_zoneDetectionYmin < position[1] && position[1] < _zoneDetectionYmax) {
			if(_zoneDetectionZmin < position[2] && position[2] < _zoneDetectionZmax) {
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
			AG_Event event;
			AG_EventArgs(&event, "%i", Controller::Action::PlayerZoneDetectorActivated);
			Controller::executeAction(&event);

		}
		else if(!isNow && wasBefore) {	// Outgoing of zone
			AG_Event event;
			AG_EventArgs(&event, "%i", Controller::Action::PlayerZoneDetectorUnactivated);
			Controller::executeAction(&event);
		}
	}

	// Mise � jour de la liste des joeurs actuellement dans la zone
	_playersInZone.clear();
	_playersInZone.insert(playersInZoneNow.begin(), playersInZoneNow.end());
}

void CheckPlayerInZone::endRefresh() {
}

void CheckPlayerInZone::Affiche() {
	if(_isVisible) {
		Fabrique::getGlUtils()->drawCube(_zoneDetectionXmin, _zoneDetectionXmax, _zoneDetectionYmin, _zoneDetectionYmax, _zoneDetectionZmin, _zoneDetectionZmax);
	}
}

void CheckPlayerInZone::AfficheSelection(float r,float v,float b) {
	glColor3f(r, v, b);
	Affiche();
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

bool CheckPlayerInZone::Lit(TiXmlElement* element, MapLogger* mapLogger) {
	// TODO
	return true;
}

bool CheckPlayerInZone::Save(TiXmlElement* element) {
	// TODO
	return true;
}

void CheckPlayerInZone::EchangeXY() {	// Echange les axes X et Y de l'objet
	float var1, var2;

	var1 = _zoneDetectionXmin;
	var2 = _zoneDetectionXmax;

	_zoneDetectionXmin = _zoneDetectionYmin;
	_zoneDetectionXmax = _zoneDetectionYmax;

	_zoneDetectionYmin = var1;
	_zoneDetectionYmax = var2;
}

void CheckPlayerInZone::EchangeXZ() {	// Echange les axes X et Y de l'objet
	float var1, var2;

	var1 = _zoneDetectionXmin;
	var2 = _zoneDetectionXmax;

	_zoneDetectionXmin = _zoneDetectionZmin;
	_zoneDetectionXmax = _zoneDetectionZmax;

	_zoneDetectionZmin = var1;
	_zoneDetectionZmax = var2;
}

void CheckPlayerInZone::EchangeYZ() {	// Echange les axes X et Y de l'objet
	float var1, var2;

	var1 = _zoneDetectionYmin;
	var2 = _zoneDetectionYmax;

	_zoneDetectionYmin = _zoneDetectionZmin;
	_zoneDetectionYmax = _zoneDetectionZmax;

	_zoneDetectionZmin = var1;
	_zoneDetectionZmax = var2;
}

void CheckPlayerInZone::Scale( float scaleX, float scaleY, float scaleZ ) {
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
