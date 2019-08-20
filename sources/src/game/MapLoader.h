/*
 * MapLoader.h
 *
 *  Created on: 18 avr. 2012
 *      Author: Erwin
 */

#ifndef MAPLOADER_H_
#define MAPLOADER_H_

#include "game/GameDto.h"

class GameDto;

class MapLoader {
	MapLoader();	// Masquage du constructeur de singleton
	virtual ~MapLoader();

	static int loadLocalGameThread(void* gameDtoVar);
	static int loadClientGameThread(void* gameDtoVar);
	static int loadServerGameThread(void* gameDtoVar);
public:
	static void launchLocalGameLoading(GameDto* gameDto);
	static void launchClientGameLoading(GameDto* gameDto);
	static void launchServerGameLoading(GameDto* gameDto);
};

#endif /* MAPLOADER_H_ */
