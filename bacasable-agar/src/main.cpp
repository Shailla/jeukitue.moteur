/*	Public domain	*/
/*
 * Test the various bindings provided by the AG_Scrollbar(3) widget using
 * arbitrary values and ranges.
 */

#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>
#include <vector>

#ifdef WIN32
#include <windows.h>
#include <io.h>
#include <direct.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <agar/config/have_opengl.h>
#include <agar/config/have_sdl.h>
#include <agar/core.h>
#include <agar/gui.h>
#include <agar/gui/cursors.h>
#include <agar/gui/sdl.h>
#include <agar/gui/opengl.h>
#include <string>

#include "ConsoleJkt.h"

using namespace std;

void createWindow(void) {

	AG_Window* window = AG_WindowNew(0);
	AG_WindowSetCaption(window, "main.cpp");

	initWidgets(window);

	AG_WindowSetGeometryAlignedPct(window, AG_WINDOW_MC, 40, 60);
	AG_WindowShow(window);
}

int main(int argc, char *argv[]) {
	/* ***************************************
	 * Initialisation AGAR
	 * **************************************/

	if (AG_InitCore("agar-scrollbar-demo", 0) == -1 || AG_InitGraphics(NULL) == -1) {
		cout << endl << AG_GetError();
		return (1);
	}

	// Gestion du clavier
	AG_BindGlobalKey(AG_KEY_ESCAPE, AG_KEYMOD_ANY, AG_QuitGUI);
	AG_BindGlobalKey(AG_KEY_F8, AG_KEYMOD_ANY, AG_ViewCapture);

	createWindow();

	AG_EventLoop();
	AG_Destroy();

	return (0);
}
