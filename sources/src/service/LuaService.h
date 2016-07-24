/*
 * LuaService.h
 *
 *  Created on: 29 avr. 2012
 *      Author: Erwin
 */

#ifndef LUASERVICE_H_
#define LUASERVICE_H_

#include <vector>
#include <string>

namespace jkt {

/**
 * Service class providing access to MAP information.
 */
class LuaService {
public:
	LuaService();
	virtual ~LuaService();

	/** Read the list of the ASE files in the ASE directory. */
	static void loadLuaDirectoryContent(std::vector<std::string>& content);
};

} /* namespace jkt */

#endif /* MAPSERVICE_H_ */
