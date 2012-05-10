/*
 * LuaService.cpp
 *
 *  Created on: 29 avr. 2012
 *      Author: Erwin
 */

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
	CFindFolder folder(PLUGINS_DIRECTORY, 0, 0);
	folder.nbr();   // TODO : Cette ligne ne sert à rien, mais lorsqu'elle n'est pas présente il y a un bug
	folder.reset();

	string luaFileName;

	while(folder.findNext(luaFileName)) {
		if((luaFileName!="..") && (luaFileName!=".") && CFindFolder::isFolder(string(PLUGINS_DIRECTORY).append(luaFileName))) {
			content.push_back(luaFileName);
		}
	}
}

} /* Namespace JktService */
