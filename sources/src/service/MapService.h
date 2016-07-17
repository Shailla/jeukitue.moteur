/*
 * MapService.h
 *
 *  Created on: 29 avr. 2012
 *      Author: Erwin
 */

#ifndef MAPSERVICE_H_
#define MAPSERVICE_H_

#include <vector>

#include "service/dto/AseFileInformationDto.h"
#include "service/dto/MapInformationDto.h"

/**
 * Service class providing access to MAP information.
 */
class MapService {
	MapService();
public:
	virtual ~MapService();

	/** Read the list of the ASE files in the ASE directory. */
	static void loadAseDirectoryContent(std::vector<AseFileInformationDto>& content);

	/** Read the list of the MAP files in the ASE directory. */
	static void loadMapDirectoryContent(std::vector<MapInformationDto>& content);
};

#endif /* MAPSERVICE_H_ */
