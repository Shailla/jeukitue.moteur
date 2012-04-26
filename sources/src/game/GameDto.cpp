/*
 * GameDto.cpp
 *
 *  Created on: 18 avr. 2012
 *      Author: Erwin
 */

#include "game/GameDto.h"

GameDto::GameDto(const string& mapName) : _mapName(mapName) {
	_playersMaxNumber = 0;
	_map = NULL;
	_erwin = NULL;
}

GameDto::~GameDto() {
}

CPlayer* GameDto::getErwin() {
	return _erwin;
}

void GameDto::setErwin(CPlayer* erwin) {
	_erwin = erwin;
}

string GameDto::getMapName() {
	return _mapName;
}

int GameDto::getPlayersMaxNumber() {
	return _playersMaxNumber;
}

void GameDto::setPlayersMaxNumber(int playersMaxNumber) {
	_playersMaxNumber = playersMaxNumber;
}

vector<CPlayer*> GameDto::getPlayers() {
	return _players;
}

CMap* GameDto::getMap() {
	return _map;
}

void GameDto::setMap(CMap* map) {
	_map = map;
}
