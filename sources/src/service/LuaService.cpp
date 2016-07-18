/*
 * LuaService.cpp
 *
 *  Created on: 29 avr. 2012
 *      Author: Erwin
 */

#include <iostream>	// for std::cout
#include "boost/filesystem/operations.hpp" // includes boost/filesystem/path.hpp
#include "boost/filesystem/fstream.hpp"

using namespace boost::filesystem;

#include "util/Trace.h"
#include "util/FindFolder.h"

#include "service/LuaService.h"

using namespace std;

extern const char* PLUGINS_DIRECTORY;

namespace JktService {

LuaService::LuaService() {
}

LuaService::~LuaService() {
}

void LuaService::loadLuaDirectoryContent(vector<string>& content) {
	path pluginPath(PLUGINS_DIRECTORY);

	if(!exists(pluginPath)) {
		LOGERROR(("Le répertoire %s n'existe pas", PLUGINS_DIRECTORY));
		return;
	}

	directory_iterator end_itr; // default construction yields past-the-end

	for ( directory_iterator itr( pluginPath ); itr != end_itr; ++itr ) {
		if ( is_directory( *itr ) ) {
			content.push_back(itr->path().filename().string());
		}
	}
}

} /* Namespace JktService */
