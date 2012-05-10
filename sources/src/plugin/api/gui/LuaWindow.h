/*
 * LuaWindow.h
 *
 *  Created on: 8 mai 2012
 *      Author: Erwin
 */

#ifndef LUAWINDOW_H_
#define LUAWINDOW_H_

#include <agar/core.h>
#include <agar/gui.h>

namespace JktPlugin {

class LuaWindow {
	AG_Window* _window;
public:
	LuaWindow();
	virtual ~LuaWindow();

protected:
	void setSize(const int x, const int y);
	void setTitle(const char titre[]);
	void show();
	void hide();
};

} /* namespace JktPlugin */
#endif /* LUAWINDOW_H_ */
