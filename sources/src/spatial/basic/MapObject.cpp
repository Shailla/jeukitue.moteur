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

void MapObject::refresh(CGame* game) {
	LOGWARN(("Use of undefined method"));
}

void MapObject::Affiche() {
	LOGWARN(("Use of undefined method"));
}

void MapObject::AfficheSelection(float r,float v,float b){
	LOGWARN(("Use of undefined method"));
}

void MapObject::initGL() {
	LOGWARN(("Use of undefined method"));
}

void MapObject::freeGL() {
	LOGWARN(("Use of undefined method"));
}

void MapObject::EchangeXY() {
	LOGWARN(("Use of undefined method"));
}

void MapObject::EchangeXZ() {
	LOGWARN(("Use of undefined method"));
}

void MapObject::EchangeYZ() {
	LOGWARN(("Use of undefined method"));
}

void MapObject::Scale(float scaleX, float scaleY, float scaleZ) {
	LOGWARN(("Use of undefined method"));
}

void MapObject::translate(float x, float y, float z) {
	LOGWARN(("Use of undefined method"));
}

void MapObject::GereContactPlayer(float positionPlayer[3], CPlayer *player) {
	LOGWARN(("Use of undefined method"));
}

float MapObject::GereLaserPlayer(float pos[3], CV3D& Dir, float dist) {
	LOGWARN(("Use of undefined method"));
}

bool MapObject::Contact(const float pos[3], float dist) {
	LOGWARN(("Use of undefined method"));
}

} /* namespace jkt */
