/*
 * LuaGlobalMethods.h
 *
 *  Created on: 20 mai 2012
 *      Author: Erwin
 */

#ifndef LUAGLOBALMETHODS_H_
#define LUAGLOBALMETHODS_H_

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

namespace JktPlugin {

class LuaGlobalMethods {
public:
	LuaGlobalMethods();
	virtual ~LuaGlobalMethods();

	static int log(lua_State* L);
};

} /* namespace JktPlugin */
#endif /* LUAGLOBALMETHODS_H_ */
