/*
 * PlayersService.h
 *
 *  Created on: 3 mars 2016
 *      Author: VGDJ7997
 */

#ifndef SRC_SERVICE_PLAYERSSERVICE_H_
#define SRC_SERVICE_PLAYERSSERVICE_H_

#include <vector>

#include "service/dto/PlayerInformationDto.h"

using namespace std;

class PlayersService {
public:
	PlayersService();
	virtual ~PlayersService();

	static void getPlayers(vector<PlayerInformationDto>& players);
};

#endif /* SRC_SERVICE_PLAYERSSERVICE_H_ */
