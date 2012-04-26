/*
 * MapLoader.h
 *
 *  Created on: 18 avr. 2012
 *      Author: Erwin
 */

#ifndef MAPLOADER_H_
#define MAPLOADER_H_

#include <string>

#include "game/GameDto.h"

using namespace std;

class MapLoader {
	MapLoader();	// Masquage du constructeur de singleton
	virtual ~MapLoader();

	static int loadGameThread(void* gameDtoVar);
public:
	static void launcheGameLoading(GameDto* gameDto);
};

#endif /* MAPLOADER_H_ */
