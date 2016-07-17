/*
 * MapService.cpp
 *
 *  Created on: 29 avr. 2012
 *      Author: Erwin
 */

#include "boost/filesystem/operations.hpp" // includes boost/filesystem/path.hpp
#include "boost/filesystem/fstream.hpp"
#include <iostream>                        // for std::cout
using namespace boost::filesystem;

#include "util/Trace.h"
#include "util/StringUtils.h"
#include "ressource/RessourceConstantes.h"

#include "service/MapService.h"
#include "service/dto/AseFileInformationDto.h"

using namespace std;

MapService::MapService() {
}

MapService::~MapService() {
}

void MapService::loadAseDirectoryContent(vector<AseFileInformationDto>& content) {
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
			AseFileInformationDto dto;

			string aseFileFullName = itr->path().filename().string();

			dto.setAseFileFullName(aseFileFullName);													// Full ASE file name
			dto.setAseFileMinimalName( string(aseFileFullName.begin(), aseFileFullName.end() - strlen(ASE_EXTENSION)) );	// ASE file name without the extension

			content.push_back(dto);
		}
	}
}

void MapService::loadMapDirectoryContent(vector<MapInformationDto>& content) {
	path mapPath(MAP_DIRECTORY);

	if(!exists(mapPath)) {
		LOGERROR(("Le répertoire %s n'existe pas", MAP_DIRECTORY));
		return;
	}

	directory_iterator end_itr; // default construction yields past-the-end

	for ( directory_iterator itr( mapPath ); itr != end_itr; ++itr ) {
		if ( is_directory( *itr ) ) {
			// Ignore directories
		}
		else {
			string filename = itr->path().string();

			if( JktUtils::StringUtils::isFinishedWith(filename, MAP_EXTENSION) ) {
				MapInformationDto dto;

				string mapFileFullName = itr->path().filename().string();

				dto.setMapFileFullName(mapFileFullName);													// Full MAP file name
				dto.setMapFileMinimalName(string(mapFileFullName.begin(), mapFileFullName.end() - strlen(MAP_EXTENSION)));	// MAP file name without the extension

				content.push_back(dto);
			}
		}
	}
}
