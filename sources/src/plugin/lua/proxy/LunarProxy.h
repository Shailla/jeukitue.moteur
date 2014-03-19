/*
 * LunarProxy.h
 *
 *  Created on: 12 févr. 2014
 *      Author: vgdj7997
 */

#ifndef LUNARPROXY_H_
#define LUNARPROXY_H_

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

namespace JktPlugin {

class LunarProxy {
public:
	LunarProxy();
	virtual ~LunarProxy();

	virtual int push(lua_State* L) = 0;
};

} /* namespace JktPlugin */

#endif /* LUNARPROXY_H_ */
