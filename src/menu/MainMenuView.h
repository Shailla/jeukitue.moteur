#ifndef __JKT__MAINMENUVIEW_H
#define __JKT__MAINMENUVIEW_H

#include <agar/core.h>
#include <agar/gui.h>

#include "menu/View.h"

class Viewer;

class MainMenuView : public View
{
    AG_Button* m_buttonOuvrirScene;
	AG_Button* m_buttonMultijoueurs;
    AG_Button* m_buttonConfiguration;
    AG_Button* m_buttonQuitter;
public:
    MainMenuView(const AG_EventFn controllerCallback, Viewer* agarView);
    ~MainMenuView(void);
};

#endif
