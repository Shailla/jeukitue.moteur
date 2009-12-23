#ifndef __JKT__MAINMENUVIEW_H
#define __JKT__MAINMENUVIEW_H

#include <agar/core.h>
#include <agar/gui.h>

class MainMenuView
{
    AG_Window *m_window;
    AG_Button* m_buttonOuvrirScene;
    AG_Button* m_buttonCentralisateur;
    AG_Button* m_buttonQuitter;
public:
    MainMenuView(const AG_EventFn controllerCallback);
    ~MainMenuView(void);

    void show(void);
    void hide(void);
};

#endif
