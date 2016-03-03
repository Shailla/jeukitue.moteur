/*
 * PlayersService.cpp
 *
 *  Created on: 3 mars 2016
 *      Author: VGDJ7997
 */

#include "main/Player.h"
#include "main/Game.h"

#include "service/PlayersService.h"

extern CGame Game;

PlayersService::PlayersService() {
}

PlayersService::~PlayersService() {
}

void PlayersService::getPlayers(vector<PlayerInformationDto>& players) {
	int curseur = -1;
	CPlayer* player;

	while((player = Game.nextPlayer(curseur))) {
		PlayerInformationDto dto;
		dto.setName(player->nom());

		players.push_back(dto);
	}
}
