
#include <agar/core.h>
#include <agar/gui.h>
#include <agar/gui/style.h>

#include "menu/View.h"
#include "menu/Controller.h"

#include "menu/MainMenuView.h"

#include <sstream>

MainMenuView::MainMenuView(const AG_EventFn controllerCallback, Viewer* agarView)
:View(controllerCallback)
{
	m_window = AG_WindowNew(AG_WINDOW_NOBUTTONS|AG_WINDOW_NOMOVE);
	AG_WindowSetCaption(m_window, "Menu principal");

    _buttonOuvrirScene = AG_ButtonNewFn(m_window, 0, "Ouvrir une scene", controllerCallback, "%i", Controller::OpenSceneAction);
	_buttonMultijoueurs = AG_ButtonNewFn(m_window, 0, "Multi-joueurs", controllerCallback, "%i", Controller::MultijoueursAction);
	_buttonConfiguration = AG_ButtonNewFn(m_window, 0, "Configuration", controllerCallback, "%i", Controller::ConfigurationAction);
	_About = AG_ButtonNewFn(m_window, 0, "A propos", controllerCallback, "%i", Controller::AboutAction);
	_debug = AG_ButtonNewFn(m_window, 0, "Debug", controllerCallback, "%i", Controller::DebugAction);

    AG_SeparatorNewHoriz(m_window);
    _buttonQuitter = AG_ButtonNewFn(m_window, 0, "Quitter", controllerCallback, "%i", Controller::QuitAction);

    AG_ExpandHoriz(_buttonOuvrirScene);
	AG_ExpandHoriz(_buttonMultijoueurs);
    AG_ExpandHoriz(_buttonConfiguration);
    AG_ExpandHoriz(_About);
    AG_ExpandHoriz(_debug);
    AG_ExpandHoriz(_buttonQuitter);

    AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_MC, 25, 50);
    hide();
}



MainMenuView::~MainMenuView(void)
{
}
