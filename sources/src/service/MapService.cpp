/*
 * MapService.cpp
 *
 *  Created on: 29 avr. 2012
 *      Author: Erwin
 */

#include "service/dto/AseFileInformationDto.h"
#include "util/FindFolder.h"

#include "service/MapService.h"

extern const char* MAP_DIRECTORY;

MapService::MapService() {
}

MapService::~MapService() {
}

void MapService::loadAseDirectoryContent(vector<AseFileInformationDto>& content) {

	CFindFolder folder( "./ASE/", 0, ".ASE" );
	folder.nbr();   // TODO : Cette ligne ne sert à rien, mais lorsqu'elle n'est pas présente il y a un bug
	folder.reset();

	string aseFileFullName;

	while(folder.findNext(aseFileFullName))
	{
		AseFileInformationDto dto;

		dto.setAseFileFullName(aseFileFullName);													// Full ASE file name
		dto.setAseFileMinimalName(aseFileFullName.erase( aseFileFullName.find_last_of( "." ) ));	// ASE file name without the extension

		content.push_back(dto);
	}
}

void MapService::loadMapDirectoryContent(vector<MapInformationDto>& content) {

	CFindFolder folder( MAP_DIRECTORY, 0, ".map.xml" );
	folder.nbr();   // TODO : Cette ligne ne sert à rien, mais lorsqu'elle n'est pas présente il y a un bug
	folder.reset();

	string mapFileFullName;

	while(folder.findNext(mapFileFullName)) {
		MapInformationDto dto;

		dto.setMapFileFullName(mapFileFullName);													// Full ASE file name

		dto.setMapFileMinimalName(mapFileFullName.erase(mapFileFullName.find_last_of(".")).erase(mapFileFullName.find_last_of(".")));	// ASE file name without the extension

		content.push_back(dto);
	}
}
