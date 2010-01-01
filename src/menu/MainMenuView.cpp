
#include <agar/core.h>
#include <agar/gui.h>
#include <agar/gui/style.h>

#include "menu/View.h"
#include "menu/Controller.h"
#include "menu/Viewer.h"

#include "menu/MainMenuView.h"

MainMenuView::MainMenuView(const AG_EventFn controllerCallback, Viewer* agarView)
:View(controllerCallback)
{
	m_window = AG_WindowNew(0);
    AG_WindowSetCaption(m_window, "Menu principal");
	//AG_LabelNewStatic(m_window, 0, "Hello, world!");

    m_buttonOuvrirScene = AG_ButtonNewFn(m_window, 0, "Ouvrir une scene", controllerCallback, "%d", Controller::OpenSceneAction);
    m_buttonCentralisateur = AG_ButtonNewFn(m_window, 0, "Centralisateur", controllerCallback, "%d", Controller::OpenCentralisateurAction);
    AG_SeparatorNewHoriz(m_window);
    m_buttonQuitter = AG_ButtonNewFn(m_window, 0, "Quitter", controllerCallback, "%d", Controller::QuitAction);

    AG_ExpandHoriz(m_buttonOuvrirScene);
    AG_ExpandHoriz(m_buttonCentralisateur);
    AG_ExpandHoriz(m_buttonQuitter);

	AG_SetStyle(m_window, agarView->getDefaultWindowStyle());

    AG_WindowSetGeometryAlignedPct(m_window, AG_WINDOW_MC, 50, 50);
	AG_WindowShow(m_window);
    show();
}

MainMenuView::~MainMenuView(void)
{
}
