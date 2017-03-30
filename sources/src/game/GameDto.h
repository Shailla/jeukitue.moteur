/*
 * GameDto.h
 *
 *  Created on: 18 avr. 2012
 *      Author: Erwin
 */

#ifndef GAMEDTO_H_
#define GAMEDTO_H_

#include <string>

#include "main/Player.h"
#include "spatial/Map.h"

using namespace jkt;

class GameDto {
	std::string _mapName;
	int _playersMaxNumber;
	std::vector<CPlayer*> _players;
	CMap* _map;
	CPlayer* _erwin;

public:
	GameDto(const std::string& mapName);
	virtual ~GameDto();

	std::string getMapName();

	int getPlayersMaxNumber();
	void setPlayersMaxNumber(int playerNumber);

	CPlayer* getErwin();
	void setErwin(CPlayer* erwin);

	std::vector<CPlayer*>& getPlayers();

	CMap* getMap();
	void setMap(CMap* map);
};

#endif /* GAMEDTO_H_ */
