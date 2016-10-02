/*
 * MapService.cpp
 *
 *  Created on: 29 avr. 2012
 *      Author: Erwin
 */

#include <iostream>                        // for std::cout
#include "boost/filesystem/operations.hpp" // includes boost/filesystem/path.hpp
#include "boost/filesystem/fstream.hpp"

using namespace boost::filesystem;

#include "util/Trace.h"
#include "util/StringUtils.h"
#include "ressource/RessourceConstantes.h"

#include "service/MapService.h"
#include <service/dto/AseInformationDto.h>

using namespace std;

MapService::MapService() {
}

MapService::~MapService() {
}

void MapService::loadAseDirectoryContent(vector<AseInformationDto>& content) {
	path asePath(ASE_DIRECTORY);

	if(!exists(asePath)) {
		LOGERROR(("Le répertoire %s n'existe pas", ASE_DIRECTORY));
		return;
	}

	directory_iterator end_itr; // default construction yields past-the-end

	for ( directory_iterator itr( asePath ); itr != end_itr; ++itr ) {
		if ( is_directory( *itr ) ) {
			// Ignore directories
		}
		else if ( itr->path().extension() == ASE_EXTENSION ) {
			AseInformationDto dto;

			string aseFileFullName = itr->path().filename().string();

			dto.setAseFileFullName(aseFileFullName);													// Full ASE file name
			dto.setAseFileMinimalName( string(aseFileFullName.begin(), aseFileFullName.end() - strlen(ASE_EXTENSION)) );	// ASE file name without the extension

			content.push_back(dto);
		}
	}
}

void MapService::loadPlayerMapDirectoryContent(vector<MapInformationDto>& content) {
	path mapPath(PLAYER_MAP_DIRECTORY);

	if(!exists(mapPath)) {
		LOGERROR(("Le répertoire %s n'existe pas", PLAYER_MAP_DIRECTORY));
		return;
	}

	directory_iterator end_itr; // default construction yields past-the-end

	for ( directory_iterator itr( mapPath ); itr != end_itr; ++itr ) {
		if ( is_directory( *itr ) ) {
			string filename = itr->path().string();

			if( jkt::StringUtils::isFinishingWith(filename, MAP_EXTENSION) ) {
				MapInformationDto dto;

				string mapFileFullName = itr->path().filename().string();

				dto.setMapFileFullName(mapFileFullName);													// Full MAP file name
				dto.setMapFileMinimalName(string(mapFileFullName.begin(), mapFileFullName.end() - strlen(MAP_EXTENSION)));	// MAP file name without the extension

				content.push_back(dto);
			}
		}
		else {
			// Ignore files
		}
	}
}

void MapService::loadMapDirectoryContent(vector<MapInformationDto>& content) {
	path mapPath(MAP_DIRECTORY);

	if(!exists(mapPath)) {
		LOGERROR(("Le répertoire '%s' n'existe pas", MAP_DIRECTORY));
		return;
	}

	directory_iterator end_itr; // default construction yields past-the-end

	for ( directory_iterator itr( mapPath ); itr != end_itr; ++itr ) {
		if ( is_directory( *itr ) ) {
			string filename = itr->path().string();

			if( jkt::StringUtils::isFinishingWith(filename, MAP_EXTENSION) ) {
				MapInformationDto dto;

				string mapFileFullName = itr->path().filename().string();

				dto.setMapFileFullName(mapFileFullName);													// Full MAP file name
				dto.setMapFileMinimalName(string(mapFileFullName.begin(), mapFileFullName.end() - strlen(MAP_EXTENSION)));	// MAP file name without the extension

				content.push_back(dto);
			}
		}
		else {
			// Ignore files
		}
	}
}
