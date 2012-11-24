#ifndef __JKT__MAINMENUVIEW_H
#define __JKT__MAINMENUVIEW_H

#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"

class Viewer;

class MainMenuView : public View
{
    AG_Button* _buttonOuvrirScene;
	AG_Button* _buttonMultijoueurs;
    AG_Button* _buttonConfiguration;
    AG_Button* _buttonQuitGame;
    AG_Button* _About;
    AG_Button* _debug;
    AG_Button* _buttonQuitter;
public:
    MainMenuView(const AG_EventFn controllerCallback);
    ~MainMenuView(void);
};

#endif
