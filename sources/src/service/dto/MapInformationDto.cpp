/*
 * MapInformationDto.cpp
 *
 *  Created on: 29 avr. 2012
 *      Author: Erwin
 */

#include "service/dto/MapInformationDto.h"

MapInformationDto::MapInformationDto() {
}

MapInformationDto::~MapInformationDto() {
}


// Setters
void MapInformationDto::setMapFileFullName(const string& mapFileFullName) {
	_mapFileFullName = mapFileFullName;
}

void MapInformationDto::setMapFileMinimalName(const string& mapFileMinimalName) {
	_mapFileMinimalName = mapFileMinimalName;
}


// Getters
string MapInformationDto::getMapFileFullName() {
	return _mapFileFullName;
}

string MapInformationDto::getMapFileMinimalName() {
	return _mapFileMinimalName;
}

