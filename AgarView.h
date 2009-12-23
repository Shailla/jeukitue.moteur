#ifndef __JKT__AGARVIEW_H
#define __JKT__AGARVIEW_H

#include "MainMenuView.h"
#include "OpenCentralisateurView.h"

class AgarView
{
    bool m_isVisible;
    MainMenuView* m_mainMenuView;
    OpenCentralisateurView* m_openCentralisateurView;
public:
    AgarView(AG_EventFn controllerCallback);
    ~AgarView(void);
    void draw(void);
    void SetVisible(bool);
    bool IsVisible(void);

    void showMainMenu(void);
    void showOpenCentralisateur(void);

    OpenCentralisateurView* getOpenCentralisateurView(void);
};

#endif
