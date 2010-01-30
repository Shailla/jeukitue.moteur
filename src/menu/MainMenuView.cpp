
#include <agar/core.h>
#include <agar/gui.h>
#include <agar/gui/style.h>

#include "menu/View.h"
#include "menu/Controller.h"

#include "menu/MainMenuView.h"

MainMenuView::MainMenuView(const AG_EventFn controllerCallback, Viewer* agarView)
:View(controllerCallback)
{
	m_window = AG_WindowNew(AG_WINDOW_NOBUTTONS|AG_WINDOW_NOMOVE);
    AG_WindowSetCaption(m_window, "Menu principal");

    m_buttonOuvrirScene = AG_ButtonNewFn(m_window, 0, "Ouvrir une scene", controllerCallback, "%i", Controller::OpenSceneAction);
	m_buttonMultijoueurs = AG_ButtonNewFn(m_window, 0, "Multi-joueurs", controllerCallback, "%i", Controller::MultijoueursAction);
	m_buttonConfiguration = AG_ButtonNewFn(m_window, 0, "Configuration", controllerCallback, "%i", Controller::ConfigurationAction);
    AG_SeparatorNewHoriz(m_window);
    m_buttonQuitter = AG_ButtonNewFn(m_window, 0, "Quitter", controllerCallback, "%i", Controller::QuitAction);

    AG_ExpandHoriz(m_buttonOuvrirScene);
	AG_ExpandHoriz(m_buttonMultijoueurs);
    AG_ExpandHoriz(m_buttonConfiguration);
    AG_ExpandHoriz(m_buttonQuitter);

    AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_MC, 50, 50);
	AG_WindowShow(m_window);
    show();
}

MainMenuView::~MainMenuView(void)
{
}
