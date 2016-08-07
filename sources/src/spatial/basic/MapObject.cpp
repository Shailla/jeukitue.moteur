/*
 * MapObject.cpp
 *
 *  Created on: 29 juil. 2016
 *      Author: VGDJ7997
 */

#include "util/Trace.h"

#include "spatial/basic/MapObject.h"

namespace jkt {

MapObject::MapObject(CMap* map) : Object(map) {
}

MapObject::MapObject(const MapObject& other) : Object(other) {
}

MapObject::~MapObject() {
}

void MapObject::beginRefresh() {
	LOGWARN(("Use of undefined method '%s'", __FUNCTION__));
}

void MapObject::refresh(CGame* game) {
	LOGWARN(("Use of undefined method '%s'", __FUNCTION__));
}

void MapObject::endRefresh() {
	LOGWARN(("Use of undefined method '%s'", __FUNCTION__));
}

void MapObject::Affiche() {
	LOGWARN(("Use of undefined method '%s'", __FUNCTION__));
}

void MapObject::AfficheSelection(float r,float v,float b){
	LOGWARN(("Use of undefined method '%s'", __FUNCTION__));
}

void MapObject::initGL() {
	LOGWARN(("Use of undefined method '%s'", __FUNCTION__));
}

void MapObject::freeGL() {
	LOGWARN(("Use of undefined method '%s'", __FUNCTION__));
}

void MapObject::EchangeXY() {
	LOGWARN(("Use of undefined method '%s'", __FUNCTION__));
}

void MapObject::EchangeXZ() {
	LOGWARN(("Use of undefined method '%s'", __FUNCTION__));
}

void MapObject::EchangeYZ() {
	LOGWARN(("Use of undefined method '%s'", __FUNCTION__));
}

void MapObject::Scale(float scaleX, float scaleY, float scaleZ) {
	LOGWARN(("Use of undefined method '%s'", __FUNCTION__));
}

void MapObject::translate(float x, float y, float z) {
	LOGWARN(("Use of undefined method '%s'", __FUNCTION__));
}

void MapObject::GereContactPlayer(float positionPlayer[3], CPlayer *player) {
	LOGWARN(("Use of undefined method '%s'", __FUNCTION__));
}

float MapObject::GereLaserPlayer(float pos[3], CV3D& Dir, float dist) {
	LOGWARN(("Use of undefined method '%s'", __FUNCTION__));
}

bool MapObject::Contact(const float pos[3], float dist) {
	LOGWARN(("Use of undefined method '%s'", __FUNCTION__));
}

} /* namespace jkt */
